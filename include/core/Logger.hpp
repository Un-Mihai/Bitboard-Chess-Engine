#pragma once

#include<iostream>

enum ErrorLevel{
    INFO,
    WARNING,
    ERROR
};

inline void log(ErrorLevel level, const char* message){
    switch (level){
        case INFO:
            std::cout << "[INFO]: " << message << '\n';
            break;
        case WARNING:
            std::cout << "[WARNING]: " << message << '\n';
            break;
        case ERROR:
            std::cout << "[ERROR]: " << message << '\n';
            break;
    }
}

inline void info(const char* msg) { log(INFO, msg); }
inline void warn(const char* msg) { log(WARNING, msg); }
inline void error(const char* msg) { log(ERROR, msg); }