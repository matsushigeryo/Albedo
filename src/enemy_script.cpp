#include "enemy_script.h"
#include <iostream>
#include <pugixml/pugiconfig.hpp>
#include "global.h"

namespace game {

    // 文字列からパラメータ構造体を作る関数
    Param Param::Parse(const std::string &str) {
        Param param;
        if (!str.empty() && str[0] == '$') {
            // 変数参照の場合
            param.is_variable_ = true;
            param.variable_name_ = str.substr(1); // '$'を除去したうえで変数名を保存
        } else {
            // リテラルの場合
            param.is_variable_ = false;
            try {
                param.value_ = std::stof(str); // floatに変換して格納
            } catch (const std::exception &) {
                log.Print("EnemyScript: パラメータ\"%s\"は不正な書式です", str.c_str());
            }
        }
        return param;
    }

    // パラメータの値を評価して取り出す関数
    float Param::Evaluate(const std::unordered_map<std::string, float> &variables) const {
        if (is_variable_) {
            auto it = variables.find(variable_name_);
            if (it != variables.end()) {
                return it->second;
            } else {
                log.Print("EnemyScript: ユーザー変数\"$%s\"は不明です", variable_name_.c_str());
                return 0.0f;
            }
        }
        return value_; // リテラルならそのまま値を返す
    }

    EnemyScriptData::EnemyScriptData(const std::string &xml_path)
        : xml_path_{xml_path} {
    }

    // XMLファイルのパース
    bool EnemyScriptData::Parse() {

        // XMLファイルを開く
        pugi::xml_document document;
        if (!document.load_file(xml_path_.c_str())) {
            return false; // XMLファイルの読み込みに失敗
        }

        // スクリプト名を取得
        pugi::xml_node root = document.child("EnemyScript");
        name_ = root.attribute("name").as_string();

        // 命令群をパースする
        commands_.clear();
        if (!ParseChildren(root, commands_)) {
            return false;
        }

        log.Print("EnemyScript: XMLスクリプトを読み込みました: %s", xml_path_.c_str());
        return true;
    }

    // 特定ノードの子要素を命令群としてパースする
    bool EnemyScriptData::ParseChildren(pugi::xml_node parent_node, std::vector<Command> &out_commands) {
        for (pugi::xml_node node : parent_node.children()) {
            std::string name = node.name();

            // Wait命令
            if (name == "Wait") {
                Param frames = Param::Parse(node.attribute("frames").as_string("0"));
                out_commands.push_back({CommandWait(frames)});
            }

            // Repeat命令
            else if (name == "Repeat") {
                Param count = Param::Parse(node.attribute("count").as_string("0"));
                std::vector<Command> child_commands;
                if (!ParseChildren(node, child_commands)) { // 子要素群をパースして命令が持つ
                    return false;
                }
                out_commands.push_back({CommandRepeat(count, child_commands)});
            }

            // SetVar命令
            else if (name == "SetVar") {
                std::string name = node.attribute("name").as_string("");
                Param value = Param::Parse(node.attribute("value").as_string("0"));
                out_commands.push_back({CommandSetVar(name, value)});
            }

            // AddVar命令
            else if (name == "AddVar") {
                std::string name = node.attribute("name").as_string("");
                Param value = Param::Parse(node.attribute("value").as_string("0"));
                out_commands.push_back({CommandAddVar(name, value)});
            }

            // Print命令
            else if (name == "Print") {
                std::string message = node.attribute("message").as_string("");
                out_commands.push_back({CommandPrint(message)});
            }

            // Log命令
            else if (name == "Log") {
                Param value = Param::Parse(node.attribute("value").as_string("0"));
                out_commands.push_back({CommandLog(value)});
            }

            else {
                log.Print("EnemyScript: XMLスクリプトのパース中に未定義の命令が見つかりました: %s", xml_path_.c_str());
                return false;
            }
        }

        return true;
    }

    // 任意のスクリプトデータを取得する
    std::shared_ptr<const EnemyScriptData> EnemyScriptManager::Get(const std::string &xml_path) {

        // パスが一致するスクリプトデータを検索
        auto it = script_map_.find(xml_path);
        if (it != script_map_.end()) return it->second; // 見つかったら返す

        // 見つからなかった場合はファイルを読み込む
        auto data = std::make_shared<EnemyScriptData>(xml_path);
        if (!data->Parse()) {
            log.Print("EnemyScript: XMLスクリプトの読み込みに失敗しました: %s", xml_path.c_str());
        } else {
            script_map_[xml_path] = data;
            return data;
        }
    }

    // スクリプトを実行する
    void EnemyScriptRunner::Update() {
        // 既に実行終了していた場合はスキップ
        if (IsFinished()) {
            return;
        }

        // 待機中の場合は待ち時間タイマーを減らして終了
        if (waiting_timer_ > 0) {
            waiting_timer_--;
            return;
        }

        // 待機時間が発生するか終端に達するまで命令を連続実行する
        while (!IsFinished() && waiting_timer_ == 0) {

            ExecutionFrame &current_frame = stack_.back();

            // 現在のブロックの終端に達した場合の処理
            if (current_frame.step_index_ >= current_frame.commands_ptr_->size()) {
                if (current_frame.repeat_count_left_ > 1) {
                    // ループ回数が残っているなら先頭に戻す
                    current_frame.repeat_count_left_--;
                    current_frame.step_index_ = 0;
                } else {
                    stack_.pop_back();
                }
                continue;
            }

            // 実行するコマンドを取得
            const Command &command_variant = (*current_frame.commands_ptr_).at(current_frame.step_index_);

            // Wait命令
            if (auto *command = std::get_if<CommandWait>(&command_variant.data_)) {
                waiting_timer_ = command->frames_.Evaluate(variables_);
                current_frame.step_index_++;
                break;
            }

            // Repeat命令
            else if (auto *command = std::get_if<CommandRepeat>(&command_variant.data_)) {
                current_frame.step_index_++;
                stack_.push_back(ExecutionFrame(&command->child_commands_, 0, command->total_count_.Evaluate(variables_))); // 子リストをスタックに積んで遷移
            }

            // SetVar命令
            else if (auto *command = std::get_if<CommandSetVar>(&command_variant.data_)) {
                variables_[command->name_] = command->value_.Evaluate(variables_);
                current_frame.step_index_++;
            }

            // AddVar命令
            else if (auto *command = std::get_if<CommandAddVar>(&command_variant.data_)) {
                variables_[command->name_] += command->value_.Evaluate(variables_);
                current_frame.step_index_++;
            }

            // Print命令
            else if (auto *command = std::get_if<CommandPrint>(&command_variant.data_)) {
                std::cout << command->message_ << std::endl;
                current_frame.step_index_++;
            }

            // Log命令
            else if (auto *command = std::get_if<CommandLog>(&command_variant.data_)) {
                if (command->value_.is_variable_) {
                    std::cout << "$" << command->value_.variable_name_ << " = " << command->value_.Evaluate(variables_) << std::endl;
                } else {
                    std::cout << command->value_.Evaluate(variables_) << std::endl;
                }
                current_frame.step_index_++;
            }

            else {
                break;
            }
        }
    }

    // 実行が終了しているかどうかを返す
    bool EnemyScriptRunner::IsFinished() const {
        return stack_.empty();
    }
}