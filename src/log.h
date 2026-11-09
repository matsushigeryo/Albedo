#ifndef LOG_H_
#define LOG_H_

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <string>
#include "game.h"

namespace game {
    class Log {
      public:
        Log(std::string output_path);
        ~Log();

        template <typename... Args>
        bool Print(const char *format, Args... args) {
            if (output_file_.is_open()) {
                if (snprintf(buf_, sizeof(buf_), format, args...) < sizeof(buf_)) {
                    output_file_ << std::setw(8) << std::setfill('0') << GetGameTime() << "  " << buf_ << std::endl;
                    return true;
                }
            }
            return false;
        }

      private:
        std::ofstream output_file_;
        char buf_[256];
    };
}

#endif