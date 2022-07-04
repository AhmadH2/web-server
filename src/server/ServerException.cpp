#include "ServerException.h"

std::map<ExceptionTypes, std::string> ServerException::excepMessage = {
    {ExceptionTypes::ARGUMENTS, "Exception: Arguments are not suitable!"},
    {ExceptionTypes::UNDEFIED_METHOD, "Undefied Http method!"},
    {ExceptionTypes::NOT_EXIST, "Element Not exist"}
};

ServerException::ServerException(ExceptionTypes excep) {
    m_message = excepMessage.at(excep);
}

std::string ServerException::message() {
    return m_message;
}

std::string ServerException::what() {
    return m_message;
}

