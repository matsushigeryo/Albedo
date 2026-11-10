#ifndef ENEMY_SCRIPT_H_
#define ENEMY_SCRIPT_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <pugixml/pugixml.hpp>

namespace game {
    struct Command;

    struct Param {
        bool is_variable_ = false;
        float value_ = 0.0f;
        std::string variable_name_ = "";

        static Param Parse(const std::string &str);
        float Evaluate(const std::unordered_map<std::string, float> &variables) const;
    };

    struct CommandWait {
        CommandWait(Param frames)
            : frames_{frames} {};
        Param frames_;
    };

    struct CommandRepeat {
        CommandRepeat(Param total_count, const std::vector<Command> &child_commands)
            : total_count_{total_count}, child_commands_{child_commands} {}
        Param total_count_;
        std::vector<Command> child_commands_{};
    };

    struct CommandSetVar {
        CommandSetVar(const std::string &name, Param value)
            : name_{name}, value_{value} {}
        std::string name_;
        Param value_;
    };

    struct CommandAddVar {
        CommandAddVar(const std::string &name, Param value)
            : name_{name}, value_{value} {}
        std::string name_;
        Param value_;
    };

    struct CommandPrint {
        CommandPrint(const std::string &message)
            : message_{message} {}
        std::string message_;
    };

    struct CommandLog {
        CommandLog(Param value)
            : value_{value} {}
        Param value_;
    };

    struct CommandFireNWay {
        CommandFireNWay(Param offset_x, Param offset_y, Param way_count, Param speed, Param angle_range, Param base_angle)
            : offset_x_{offset_x}, offset_y_{offset_y}, way_count_{way_count}, speed_{speed}, angle_range_{angle_range}, base_angle_{base_angle} {}
        Param offset_x_;
        Param offset_y_;
        Param way_count_;
        Param speed_;
        Param angle_range_;
        Param base_angle_;
    };

    struct Command {
        std::variant<CommandWait, CommandRepeat, CommandSetVar, CommandAddVar, CommandPrint, CommandLog, CommandFireNWay> data_;
    };

    // スクリプトの命令群を扱う構造体
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

    // 複数のスクリプトデータを管理する構造体
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

    // スクリプトデータを参照して命令を実行する構造体
    class EnemyScriptRunner {
      public:
        explicit EnemyScriptRunner(std::shared_ptr<const EnemyScriptData> script_data)
            : script_data_{std::move(script_data)} {
            if (script_data_ && !script_data_->commands_.empty()) {
                // ルート階層をスタックに追加
                stack_.push_back({&script_data_->commands_, 0, 1});
            }
        }

        void Update();
        bool IsFinished() const;

      private:
        std::shared_ptr<const EnemyScriptData> script_data_;
        std::vector<ExecutionFrame> stack_;
        std::unordered_map<std::string, float> variables_;
        unsigned int waiting_timer_ = 0;
    };
}

#endif