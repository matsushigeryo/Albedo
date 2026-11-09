#include "log.h"

namespace game {
    Log::Log(std::string output_path)
        : output_file_{output_path} {
        Print("Log: ログの記録を開始します");
    }

    Log::~Log() {
        if (output_file_.is_open()) {
            Print("Log: ログの記録を終了します");
            output_file_.close();
        }
    }
}