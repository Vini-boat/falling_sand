#include "utils/utils.hpp"

using namespace std;

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

string getFileContent(
    const char* path
){
    string result = "";
    ifstream file;
    file.exceptions (ifstream::failbit | ifstream::badbit);
    try{
        file.open(path);
        stringstream ss;
        ss << file.rdbuf();
        file.close();
        result = ss.str();

    }catch(ifstream::failure& e){
        cout << "ERROR::IO::FILE_NOT_SUCCESFULLY_READ" << endl;
        cout << path << endl;
    }
    return result;
}