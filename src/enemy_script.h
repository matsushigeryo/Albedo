#ifndef ENEMY_SCRIPT_H_
#define ENEMY_SCRIPT_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <pugixml/pugixml.hpp>
#include "enemy_bullet.h"

namespace game {
    namespace enemy_script {

        struct Command;

        // 変数名参照をサポートしたパラメータ構造体
        struct Param {
            bool is_variable_ = false;
            float value_ = 0.0f;
            std::string variable_name_ = "";

            static Param Parse(const std::string &str);
            float Evaluate(const std::unordered_map<std::string, float> &variables) const;
        };

        // 並列処理タスクの構造体
        struct ParallelTask {
            std::vector<Command> commands_;
        };

        // 並列処理のモードを示す列挙型
        enum class ParallelMode {
            Join, // 子要素に含まれる全てのタスクが終了するまで待つ
            Fork  // 並列処理範囲の完了を待たず、即時に次へ進む
        };

        // コマンド構造体群
        struct CommandWait {
            Param frames_;
        };

        struct CommandRepeat {
            Param total_count_;
            std::vector<Command> child_commands_{};
        };

        struct CommandParallel {
            std::vector<ParallelTask> tasks_;
            ParallelMode mode_ = ParallelMode::Join;
        };

        struct CommandSetVar {
            std::string name_;
            Param value_;
        };

        struct CommandAddVar {
            std::string name_;
            Param value_;
        };

        struct CommandGetRandom {
            std::string name_;
            Param min_;
            Param max_;
        };

        struct CommandGetAngleToPlayer {
            std::string name_;
            Param offset_x_;
            Param offset_y_;
        };

        struct CommandSetEnemyPos {
            Param x_;
            Param y_;
        };

        struct CommandGetEnemyPos {
            std::string var_x_;
            std::string var_y_;
        };

        struct CommandMoveBy {
            Param dx_;
            Param dy_;
        };

        struct CommandKillEnemy {
        };

        struct CommandPrint {
            std::string message_;
        };

        struct CommandLog {
            Param value_;
        };

        struct CommandFireNWay {
            Param offset_x_;
            Param offset_y_;
            Param way_count_;
            Param speed_;
            Param angle_range_;
            Param base_angle_;
            EnemyBulletShape shape_ = EnemyBulletShape::SmallCircle;
            EnemyBulletColor color_ = EnemyBulletColor::Red;
        };

        // コマンド構造体のラッパー
        struct Command {
            std::variant<
                CommandWait,
                CommandRepeat,
                CommandParallel,
                CommandSetVar,
                CommandAddVar,
                CommandGetRandom,
                CommandGetAngleToPlayer,
                CommandSetEnemyPos,
                CommandGetEnemyPos,
                CommandMoveBy,
                CommandKillEnemy,
                CommandPrint,
                CommandLog,
                CommandFireNWay>
                data_;
        };

        // スクリプトの命令群を扱うクラス
        class EnemyScriptData {
          public:
            EnemyScriptData(const std::string &xml_path);
            bool Parse();
            bool ParseChildren(pugi::xml_node parent_node, std::vector<Command> &out_commands);
            Param GetParamOrDefault(pugi::xml_node node, const char *attribute_name, const std::string &default_value, bool warning_log);

          public:
            std::vector<Command> commands_{};

          private:
            std::string xml_path_;
            std::string name_ = "";
        };

        // 複数のスクリプトデータを管理するクラス
        class EnemyScriptManager {
          public:
            EnemyScriptManager() = default;
            std::shared_ptr<const EnemyScriptData> Get(const std::string &xml_path);

          private:
            std::unordered_map<std::string, std::shared_ptr<const EnemyScriptData>> script_map_{};
        };

        // 一つの実行階層を管理する構造体
        struct ExecutionFrame {
            ExecutionFrame(const std::vector<Command> *commands_ptr, size_t step_index, int repeat_count)
                : commands_ptr_{commands_ptr}, step_index_{step_index}, repeat_count_left_{repeat_count} {}
            const std::vector<Command> *commands_ptr_;
            size_t step_index_;
            int repeat_count_left_;
        };

        // スクリプトを実行する際の情報受け渡し用コンテキスト構造体
        struct EnemyScriptRunnerContext {
            EnemyScriptRunnerContext(float &origin_x, float &origin_y, bool &owner_is_active)
                : origin_x_{origin_x}, origin_y_{origin_y}, owner_is_active_{owner_is_active} {}
            float &origin_x_;
            float &origin_y_;
            bool &owner_is_active_;
        };

        // 独自した軽量な実行単位
        struct Fiber {
            std::vector<ExecutionFrame> stack_;
            unsigned int waiting_timer_ = 0;
            bool is_finished_ = false;
            bool is_waiting_join_ = false;
            std::vector<Fiber> child_fibers_{}; // 並列処理中の子ファイバー
        };

        // スクリプトデータを参照して命令を実行するクラス
        class EnemyScriptRunner {
          public:
            explicit EnemyScriptRunner(std::shared_ptr<const EnemyScriptData> script_data)
                : script_data_{std::move(script_data)} {
                if (script_data_ && !script_data_->commands_.empty()) {
                    // スクリプトのルート階層をルートファイバーのスタックに追加
                    root_fiber_.stack_.push_back({&script_data_->commands_, 0, 1});
                }
            }

            void Update(EnemyScriptRunnerContext &context);
            void UpdateFiber(Fiber &fiber, EnemyScriptRunnerContext &context);
            bool IsFinished() const;

          private:
            std::shared_ptr<const EnemyScriptData> script_data_; // 参照するスクリプトデータ
            Fiber root_fiber_;                                   // 最初に実行されるルートファイバー
            std::unordered_map<std::string, float> variables_;
        };
    }
}

#endif