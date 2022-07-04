#pragma once
#include <boost/asio.hpp>
#include <exception>
#include <map>

enum ExceptionTypes {ARGUMENTS, UNDEFIED_METHOD, NOT_EXIST};

class ServerException: public virtual boost::exception {
public:
  ServerException(ExceptionTypes);
  std::string message();
  std::string what();
private:
  std::string m_message;
  static std::map<ExceptionTypes, std::string> excepMessage;
};