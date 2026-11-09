#include "enemy_script.h"
#include <iostream>
#include <pugixml/pugiconfig.hpp>
#include "global.h"

namespace game {
    EnemyScriptData::EnemyScriptData(const std::string &xml_path)
        : xml_path_{xml_path} {
    }

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

    bool EnemyScriptData::ParseChildren(pugi::xml_node parent_node, std::vector<Command> &out_commands) {
        for (pugi::xml_node node : parent_node.children()) {
            std::string name = node.name();

            // Wait命令
            if (name == "Wait") {
                int frames = node.attribute("frames").as_int(0);
                out_commands.push_back(CommandWait(frames));

            // Print命令
            } else if (name == "Print") {
                std::string message = node.attribute("message").as_string("");
                out_commands.push_back(CommandPrint(message));
            }

            else {
                log.Print("EnemyScript: XMLスクリプトのパース中に未定義の命令が見つかりました: %s", xml_path_.c_str());
                return false;
            }
        }

        return true;
    }

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

    void EnemyScriptRunner::Update() {
        if (script_data_) {
            // 実行位置が終端に達していた場合は中断
            if (current_step_ >= script_data_->commands_.size()) {
                return;
            }

            // 待ち時間タイマーが残っている場合はデクリメントして中断
            if (waiting_timer_ > 0) {
                waiting_timer_--;
                return;
            }

            // variantで取得して実際の型ごとに分岐処理
            auto command_variant = script_data_->commands_.at(current_step_);

            // Wait命令
            if (auto command = std::get_if<CommandWait>(&command_variant)) {
                waiting_timer_ = command->frames_;
            }

            // Print命令
            else if (auto command = std::get_if<CommandPrint>(&command_variant)) {
                std::cout << command->message_ << std::endl;
            }

            // 実行する位置を更新する
            current_step_++;
        }
    }
}