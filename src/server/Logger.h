#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <map>
#include <memory>

enum LogMode { TRACE, DEBUG, INFO, WARN, ERROR };



#define WrapLog(mode, message)(Logger::log(mode, message, __FILE__, __LINE__))

class Logger {
    friend class WrapLogger;
public:
    
    // Singletons should not be cloneable.
    Logger(Logger& logger) = delete;
    // Singletons should not be assignable.
    void operator = (const Logger& logger) = delete;

    // write to log file
    Logger& operator << (std::string message);
    

    void static log(LogMode, std::string message, std::string module, int line);
    

protected:
    static Logger* getInstance();
    Logger();
    ~Logger();
    
private:
    static std::ofstream m_file;
    static std::mutex m_mutex;
    static Logger* m_pinstance;
    static std::string m_fileName;
    static std::map<LogMode, std::string> mode_map;
};
