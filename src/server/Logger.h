#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
#include <chrono>
#include <map>

typedef enum LogMode { TRACE, DEBUG, INFO, WARN, ERROR } logMode;



#define WrapLog(mode, message)(Logger::getInstance()->log(mode, message, __FILE__, __LINE__))

class Logger {
public:
    static Logger* getInstance();
    // Singletons should not be cloneable.
    Logger(Logger& logger) = delete;
    // Singletons should not be assignable.
    void operator = (const Logger& logger) = delete;

    // write to log file
    Logger& operator << (std::string message);
    

    void log(LogMode, std::string message, std::string module, int line);

protected:
    Logger();
    ~Logger();
    
private:
    static std::ofstream m_stream;
    static std::mutex m_mutex;
    static Logger* m_pinstance;
    static const std::string m_fileName;
    std::map<LogMode, std::string> mode_map = {
    {LogMode::INFO, "Info"},
    {LogMode::DEBUG, "Debug"},
    {LogMode::ERROR, "Error"}
};
};
