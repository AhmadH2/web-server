#include <boost/asio.hpp>
#include <exception>
#include <map>

enum ExcepTypes {ARGUMENTS};

class myException: public virtual boost::exception {
public:
  myException(ExcepTypes);
  std::string message();
private:
  std::string m_message;
  static std::map<ExcepTypes, std::string> excepMessage;
};