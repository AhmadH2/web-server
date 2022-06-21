#include "MethodFactory.h"

MethodFactory::MethodFactory(Service* service): m_service(service) {
    m_methods_map = {
        {"get", new HttpGet(m_service)},
        {"head", new HttpHead(m_service)},
        {"echo", new HttpEcho(m_service)}
    };
}

HttpMethod* MethodFactory::getMethod(std::string method) {
    boost::to_lower(method);
    std::cout<<"method is: "<<method<<"\n";
    if(m_methods_map.count(method)) {
        return m_methods_map.at(method);
    }
    
    return nullptr;
}