#include "Logger.h"

Logger* Logger::m_pinstance{nullptr};
std::mutex Logger::m_mutex;
std::ofstream Logger::m_stream;
const std::string Logger::m_fileName = "log.txt";

Logger* Logger::getInstance() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_pinstance == nullptr) {
      m_pinstance = new Logger();
      m_stream.open(m_fileName, std::fstream::app);
    }
    return m_pinstance;
}

Logger::Logger()
  {
 
    if (m_stream.fail())
    {
      throw std::iostream::failure("Cannot open file: " + m_fileName);
    }
  }  

 Logger& Logger::operator << (std::string message) {
    std::lock_guard lock(m_mutex);
    m_stream << message << std::endl;
    return (*this);
}

void Logger::log(LogMode mode, std::string message, std::string modu, int line)
{
  std::string mode_str="";

  mode_str = mode_map.at(mode);

  std::lock_guard lock(m_mutex);
  auto start = std::chrono::system_clock::now();
  auto legacyStart = std::chrono::system_clock::to_time_t(start);
  
  std::string stime = std::ctime(&legacyStart);
  stime = stime.substr(0, stime.length()-2);
  m_stream <<stime<<", "<< mode_str<<", "<<modu<<", "<<line<<", " << message << std::endl;
}

Logger::~Logger() {
  m_stream.close();
}