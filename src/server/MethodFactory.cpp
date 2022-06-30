#include "MethodFactory.h"

Service* MethodFactory::m_service = nullptr;
std::map<std::string, HttpMethod*> MethodFactory::m_methods_map = {
        {"get", new HttpGet(m_service)},
        {"head", new HttpHead(m_service)},
        {"echo", new HttpEcho(m_service)}
    };

// MethodFactory::MethodFactory(Service* service): m_service(service) {}

HttpMethod* MethodFactory::getMethod(std::string method, Service* service) {
    boost::to_lower(method);
    m_service = service;
    m_methods_map = {
        {"get", new HttpGet(m_service)},
        {"head", new HttpHead(m_service)},
        {"echo", new HttpEcho(m_service)}
    };
    if(m_methods_map.count(method)) {
        return m_methods_map.at(method);
    }
    
    return nullptr;
}