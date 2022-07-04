#include "Logger.h"

Logger* Logger::m_pinstance{nullptr};
std::mutex Logger::m_mutex;
std::ofstream Logger::m_file;
std::string Logger::m_fileName = "./logs/log";
std::map<LogMode, std::string> Logger::mode_map = {
    {LogMode::INFO, "Info"},
    {LogMode::DEBUG, "Debug"},
    {LogMode::ERROR, "Error"},
    {LogMode::TRACE, "Trace"},
    {LogMode::WARN, "Warn"}
};

Logger* Logger::getInstance() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_pinstance == nullptr) {
      auto start = std::chrono::system_clock::now();
      auto legacyStart = std::chrono::system_clock::to_time_t(start);
      std::string stime = std::ctime(&legacyStart);
      m_fileName = "./logs/log_"+stime;
      m_fileName = m_fileName.substr(0, m_fileName.length()-7);
      std::cout<<"file name: "<<m_fileName;
      m_pinstance = new Logger();

    }
    return m_pinstance;
}

Logger::Logger() {}  

 Logger& Logger::operator << (std::string message) {
    std::lock_guard lock(m_mutex);
    m_file << message << std::endl;
    return (*this);
}

void Logger::log(LogMode mode, std::string message, std::string modu, int line)
{
  m_file.open(m_fileName, std::fstream::app);
  std::string mode_str="";

  mode_str = mode_map.at(mode);

  std::lock_guard lock(m_mutex);
  auto start = std::chrono::system_clock::now();
  auto legacyStart = std::chrono::system_clock::to_time_t(start);
  
  std::string stime = std::ctime(&legacyStart);
  stime = stime.substr(0, stime.length()-2);
  std::cout <<stime<<", "<< mode_str<<", "<<modu<<", "<<line<<", " << message << std::endl;
  m_file <<stime<<", "<< mode_str<<", "<<modu<<", "<<line<<", " << message << std::endl;
  m_file.close();
}

Logger::~Logger() {
  m_file.close();
}