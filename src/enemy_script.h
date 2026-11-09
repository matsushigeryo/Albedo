#ifndef ENEMY_SCRIPT_H_
#define ENEMY_SCRIPT_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
#include <pugixml/pugixml.hpp>

namespace game {
    struct CommandWait {
        CommandWait(int frames)
            : frames_{frames} {};
        int frames_;
    };

    struct CommandPrint {
        CommandPrint(const std::string &message)
            : message_{message} {};
        std::string message_;
    };

    struct CommandFireNWay {
        CommandFireNWay(int way_count, float speed, float angle_range)
            : way_count_{way_count}, speed_{speed}, angle_range_{angle_range} {};
        int way_count_ = 1;
        float speed_ = 0.0f;
        float angle_range_ = 0.0f;
    };

    using Command = std::variant<CommandWait, CommandPrint, CommandFireNWay>;

    class EnemyScriptData {
      public:
        EnemyScriptData(const std::string &xml_path);
        bool Parse();
        bool ParseChildren(pugi::xml_node parent_node, std::vector<Command> &out_commands);

      public:
        std::vector<Command> commands_{};

      private:
        std::string xml_path_;
        std::string name_ = "";
    };

    class EnemyScriptManager {
      public:
        EnemyScriptManager() = default;
        std::shared_ptr<const EnemyScriptData> Get(const std::string &xml_path);

      private:
        std::unordered_map<std::string, std::shared_ptr<const EnemyScriptData>> script_map_{};
    };

    class EnemyScriptRunner {
      public:
        explicit EnemyScriptRunner(std::shared_ptr<const EnemyScriptData> script_data)
            : script_data_{script_data} {
        }

        void Update();

      private:
        std::shared_ptr<const EnemyScriptData> script_data_;
        size_t current_step_ = 0;
        unsigned int waiting_timer_ = 0;
    };
}

#endif