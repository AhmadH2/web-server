#include "Exception.h"

std::map<ExcepTypes, std::string> myException::excepMessage = {
        {ExcepTypes::ARGUMENTS, "Exception: Arguments are not suitable!"}
};

myException::myException(ExcepTypes excep) {
    m_message = excepMessage.at(excep);
}

std::string myException::message() {
    return m_message;
}
