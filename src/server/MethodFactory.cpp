#include "MethodFactory.h"

std::map<std::string, HttpMethod*> MethodFactory::m_methods_map = {
    {"get", nullptr},
    {"head", nullptr},
    {"echo", nullptr}
};

HttpMethod* MethodFactory::getMethod(Service* service) {
    std::string method = service->getRequestMethod();
    boost::to_lower(method);
    m_methods_map = {
        {"get", new HttpGet(service)},
        {"head", new HttpHead(service)},
        {"echo", new HttpEcho(service)}
    };
    if(!m_methods_map.count(method)) {
        throw ServerException(ExceptionTypes::UNDEFIED_METHOD);
    }
    
    return m_methods_map.at(method);
}