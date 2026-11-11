#include "enemy_script.h"
#include <iostream>
#include <pugixml/pugiconfig.hpp>
#include "global.h"

namespace game {
    namespace enemy_script {

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
                    log.Print("EnemyScript: XMLスクリプトの実行中にエラーが発生しました: パラメータ\"%s\"は不正な書式です", str.c_str());
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
                    log.Print("EnemyScript: XMLスクリプトの実行中にエラーが発生しました: ユーザー変数\"$%s\"は不明です", variable_name_.c_str());
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
                    CommandWait command;
                    command.frames_ = GetParamOrDefault(node, "frames", "0", true);
                    out_commands.push_back({command});
                }

                // Repeat命令
                else if (name == "Repeat") {
                    CommandRepeat command;
                    command.total_count_ = GetParamOrDefault(node, "count", "0", true);
                    if (!ParseChildren(node, command.child_commands_)) { // 子要素群をパースして命令が持つ
                        return false;
                    }
                    out_commands.push_back({command});
                }

                // Parallel命令
                else if (name == "Parallel") {
                    CommandParallel command;

                    // モードを取得（デフォルトはJoin）
                    std::string mode_string = node.attribute("mode").as_string("Join");
                    if (mode_string == "Join")
                        command.mode_ = ParallelMode::Join;
                    else if (mode_string == "Fork")
                        command.mode_ = ParallelMode::Fork;

                    // 子要素群のパース
                    for (pugi::xml_node child : node.children()) {
                        std::string child_name = child.name();

                        if (child_name == "Task") {
                            ParallelTask task;
                            if (!ParseChildren(child, task.commands_)) { // Taskの中身をパース
                                return false;
                            }
                            command.tasks_.push_back(std::move(task));

                        } else {
                            // Parallelタグ直下にTask以外のタグが記述されていた場合の警告
                            log.Print("EnemyScript: <Parallel>タグの直下には<Task>のみを記述してください (見つかったタグ: <%s>)", child_name.c_str());
                            return false;
                        }
                    }

                    // 空のParallelタグに対して警告
                    if (command.tasks_.empty()) {
                        log.Print("EnemyScript: <Parallel>タグ内に実行可能な<Task>が含まれていません");
                    }

                    out_commands.push_back({command});
                }

                // SetVar命令
                else if (name == "SetVar") {
                    CommandSetVar command;
                    command.name_ = node.attribute("name").as_string("");
                    command.value_ = Param::Parse(node.attribute("value").as_string("0"));
                    out_commands.push_back({command});
                }

                // AddVar命令
                else if (name == "AddVar") {
                    CommandAddVar command;
                    command.name_ = node.attribute("name").as_string("");
                    command.value_ = Param::Parse(node.attribute("value").as_string("0"));
                    out_commands.push_back({command});
                }

                // GetRandom命令
                else if (name == "GetRandom") {
                    CommandGetRandom command;
                    command.name_ = node.attribute("name").as_string("");
                    command.min_ = GetParamOrDefault(node, "min", "0", true);
                    command.max_ = GetParamOrDefault(node, "max", "0", true);
                    out_commands.push_back({command});
                }

                // GetAngleToPlayer命令
                else if (name == "GetAngleToPlayer") {
                    CommandGetAngleToPlayer command;
                    command.name_ = node.attribute("name").as_string("");
                    command.offset_x_ = GetParamOrDefault(node, "offset_x", "0", false);
                    command.offset_y_ = GetParamOrDefault(node, "offset_y", "0", false);
                    out_commands.push_back({command});
                }

                // SetEnemyPos命令
                else if (name == "SetEnemyPos") {
                    CommandSetEnemyPos command;
                    command.x_ = GetParamOrDefault(node, "x", "0", true);
                    command.y_ = GetParamOrDefault(node, "y", "0", true);
                    out_commands.push_back({command});
                }

                // GetEnemyPos命令
                else if (name == "GetEnemyPos") {
                    CommandGetEnemyPos command;
                    command.var_x_ = node.attribute("x").as_string("");
                    command.var_y_ = node.attribute("y").as_string("");
                    out_commands.push_back({command});
                }

                // Print命令
                else if (name == "Print") {
                    CommandPrint command;
                    command.message_ = node.attribute("message").as_string("");
                    out_commands.push_back({command});
                }

                // Log命令
                else if (name == "Log") {
                    CommandLog command;
                    command.value_ = Param::Parse(node.attribute("value").as_string("0"));
                    out_commands.push_back({command});
                }

                // FireNWay命令
                else if (name == "FireNWay") {
                    CommandFireNWay command;
                    command.offset_x_ = GetParamOrDefault(node, "offset_x", "0", false);
                    command.offset_y_ = GetParamOrDefault(node, "offset_y", "0", false);
                    command.way_count_ = GetParamOrDefault(node, "way_count", "0", true);
                    command.speed_ = GetParamOrDefault(node, "speed", "0", true);
                    command.angle_range_ = GetParamOrDefault(node, "angle_range", "0", true);
                    command.base_angle_ = GetParamOrDefault(node, "base_angle", "0", true);
                    out_commands.push_back({command});
                }

                else {
                    log.Print("EnemyScript: XMLスクリプトのパース中に未定義の命令が見つかりました: %s", name.c_str());
                    return false;
                }
            }

            return true;
        }

        // XMLタグから属性を取得する
        Param EnemyScriptData::GetParamOrDefault(pugi::xml_node node, const char *attribute_name, const std::string &default_value, bool warning_log) {
            pugi::xml_attribute attr = node.attribute(attribute_name);

            // 属性が見つからなかった場合
            if (!attr) {
                if (warning_log) {
                    log.Print("EnemyScript: XMLスクリプトの<%s>タグに属性\"%s\"が含まれていません。デフォルト値\"%s\"を適応します", node.name(), attribute_name, default_value.c_str());
                }
                return Param::Parse(default_value);
            }

            return Param::Parse(attr.as_string());
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

            return nullptr;
        }

        // スクリプトを実行する
        void EnemyScriptRunner::Update(EnemyScriptRunnerContext &context) {
            if (IsFinished()) return;
            UpdateFiber(root_fiber_, context);
        }

        // ファイバーを更新する
        void EnemyScriptRunner::UpdateFiber(Fiber &fiber, EnemyScriptRunnerContext &context) {

            // 既に実行終了していた場合はスキップ
            if (fiber.is_finished_) {
                return;
            }

            // 子ファイバーの更新
            if (!fiber.child_fibers_.empty()) {
                bool all_children_finished = true;

                for (auto &child : fiber.child_fibers_) {
                    UpdateFiber(child, context);
                    if (!child.is_finished_) {
                        all_children_finished = false;
                    }
                }

                if (all_children_finished) {
                    fiber.child_fibers_.clear();
                    fiber.is_waiting_join_ = false; // 子ファイバーが全て終わったらJoin待ちを解除
                }
            }

            // 待ち時間が残っている場合は待つ
            if (fiber.waiting_timer_ > 0) {
                fiber.waiting_timer_--;
                return;
            }

            // ParallelがJoinモードかつその子ファイバーが処理中なら実行をブロック
            if (fiber.is_waiting_join_ && !fiber.child_fibers_.empty()) {
                return;
            }

            // 親ファイバーのコマンド連続実行
            while (!fiber.stack_.empty() && fiber.waiting_timer_ == 0) {

                // Join待ちに入った場合はループを抜ける
                if (fiber.is_waiting_join_ && !fiber.child_fibers_.empty()) {
                    break;
                }

                ExecutionFrame &current_frame = fiber.stack_.back();

                // 現在のブロックの終端に達した場合の処理
                if (current_frame.step_index_ >= current_frame.commands_ptr_->size()) {
                    if (current_frame.repeat_count_left_ > 1) {
                        current_frame.repeat_count_left_--;
                        current_frame.step_index_ = 0;
                    } else {
                        fiber.stack_.pop_back();
                    }
                    continue;
                }

                // 実行するコマンドを取得
                const Command &command_variant = (*current_frame.commands_ptr_).at(current_frame.step_index_);

                // Wait命令
                if (auto *command = std::get_if<CommandWait>(&command_variant.data_)) {
                    float wait_frame = command->frames_.Evaluate(variables_);
                    if (wait_frame < 0) wait_frame = 0;
                    fiber.waiting_timer_ = static_cast<unsigned int>(wait_frame);
                    current_frame.step_index_++;
                    break;
                }

                // Repeat命令
                else if (auto *command = std::get_if<CommandRepeat>(&command_variant.data_)) {
                    current_frame.step_index_++;
                    fiber.stack_.push_back(ExecutionFrame(&command->child_commands_, 0, static_cast<int>(command->total_count_.Evaluate(variables_)))); // 子リストをスタックに積んで遷移
                }

                // Parallel命令
                else if (auto *command = std::get_if<CommandParallel>(&command_variant.data_)) {
                    current_frame.step_index_++;

                    // 各タスクを子ファイバーとして生成する
                    for (const auto &task : command->tasks_) {
                        Fiber child;
                        child.stack_.push_back(ExecutionFrame(&task.commands_, 0, 1));
                        UpdateFiber(child, context); // 生成直後に即時発火
                        if (!child.is_finished_) {
                            fiber.child_fibers_.push_back(child);
                        }
                    }

                    // Joinモードなら親の停止フラグを立ててループを抜ける
                    if (command->mode_ == ParallelMode::Join && !fiber.child_fibers_.empty()) {
                        fiber.is_waiting_join_ = true;
                        break;
                    }
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

                // GetRandom命令
                else if (auto *command = std::get_if<CommandGetRandom>(&command_variant.data_)) {
                    const float min = command->min_.Evaluate(variables_);
                    const float max = command->max_.Evaluate(variables_);
                    variables_[command->name_] = min + xoshiro.GetRandomNormalized() * (max - min);
                    current_frame.step_index_++;
                }

                // GetAngleToPlayer命令
                else if (auto *command = std::get_if<CommandGetAngleToPlayer>(&command_variant.data_)) {
                    const float dy = player_ptr->GetY() - (context.origin_y_ + command->offset_y_.Evaluate(variables_));
                    const float dx = player_ptr->GetX() - (context.origin_x_ + command->offset_x_.Evaluate(variables_));
                    variables_[command->name_] = RadianToDegree(atan2f(dy, dx));
                    current_frame.step_index_++;
                }

                // SetEnemyPos命令
                else if (auto *command = std::get_if<CommandSetEnemyPos>(&command_variant.data_)) {
                    context.origin_x_ = command->x_.Evaluate(variables_);
                    context.origin_y_ = command->y_.Evaluate(variables_);
                    current_frame.step_index_++;
                }

                // GetEnemyPos命令
                else if (auto *command = std::get_if<CommandGetEnemyPos>(&command_variant.data_)) {
                    variables_[command->var_x_] = context.origin_x_;
                    variables_[command->var_y_] = context.origin_y_;
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

                // FireNWay命令
                else if (auto *command = std::get_if<CommandFireNWay>(&command_variant.data_)) {
                    const float offset_x = command->offset_x_.Evaluate(variables_);
                    const float offset_y = command->offset_y_.Evaluate(variables_);
                    const int way_count = static_cast<int>(command->way_count_.Evaluate(variables_));
                    const float speed = command->speed_.Evaluate(variables_);
                    const float base_angle = command->base_angle_.Evaluate(variables_);

                    // 1Wayの場合は基準角に1発発射するだけ
                    if (way_count == 1) {
                        enemy_bullet_manager_ptr->AddBullet(context.origin_x_ + offset_x, context.origin_y_ + offset_y, DegreeToRadian(base_angle), speed);
                    }

                    // 複数Way
                    else if (way_count >= 2) {
                        const float angle_range = command->angle_range_.Evaluate(variables_);
                        const float start_angle = base_angle - (angle_range / 2.0f);
                        const float angle_step = angle_range / (way_count - 1);
                        for (int i = 0; i < way_count; ++i) {
                            float current_angle = start_angle + (angle_step * i);
                            enemy_bullet_manager_ptr->AddBullet(context.origin_x_ + offset_x, context.origin_y_ + offset_y, DegreeToRadian(current_angle), speed);
                        }
                    }

                    current_frame.step_index_++;
                }

                else {
                    break;
                }
            }

            // スタックと子ファイバーが空ならファイバーを終了
            if (fiber.stack_.empty() && fiber.child_fibers_.empty()) {
                fiber.is_finished_ = true;
            }
        }

        // 実行が終了しているかどうかを返す
        bool EnemyScriptRunner::IsFinished() const {
            return root_fiber_.is_finished_;
        }
    }
}