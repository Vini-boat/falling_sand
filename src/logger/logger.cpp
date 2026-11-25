#include "logger.hpp"
#include <string>
#include <iostream>
#include <source_location>
#include <glad/glad.h>

using namespace std;

void Logger::info(std::string msg, std::source_location loc){
    cout << Styles::info << " INFO  " << Styles::clear;
    cout << "[" << loc.file_name() << ":" << loc.line() << "]" << ": ";
    cout << msg << endl;
};
void Logger::warn(std::string msg, std::source_location loc){
    cout << Styles::warn << " WARN  " << Styles::clear;
    cout << "[" << loc.file_name() << ":" << loc.line() << "]" << ": ";
    cout << msg << endl;
};
void Logger::error(std::string msg, std::source_location loc){
    cout << Styles::error << " ERROR " << Styles::clear;
    cout << "[" << loc.file_name() << ":" << loc.line() << "]" << ": ";
    cout << msg << endl;
};

void Logger::glCheck(const char* onde, 
    std::source_location loc
) {
    while (GLenum e = glGetError())
        _log.error(std::format("GL {}: 0x{:X}", onde, e), loc);
}