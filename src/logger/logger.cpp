#include "logger.hpp"
#include <string>
#include <iostream>
#include <source_location>


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