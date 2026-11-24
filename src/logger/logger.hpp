#pragma once

#include <string>
#include <source_location>
#include <format>

struct Logger {

    void info(std::string msg,
        std::source_location loc = std::source_location::current()
    );
    void warn(std::string msg,
        std::source_location loc = std::source_location::current()
    );
    void error(std::string msg,
        std::source_location loc = std::source_location::current()
    );
    
    private:
        struct Styles {
            static constexpr const char* clear = "\033[0m";
            static constexpr const char* info  = "\033[97m";
            static constexpr const char* warn  = "\033[33m";
            static constexpr const char* error = "\033[97;41m";
        };
};

inline Logger _log;