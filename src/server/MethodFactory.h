#pragma once
#include "httpMethod.h"
#include "httpGet.h"
#include "httpHead.h"
#include "httpEcho.h"
#include <boost/algorithm/string.hpp>
#include <string>

class MethodFactory {
    
public:
    static HttpMethod* getMethod(std::string method, Service* service);
    // MethodFactory(Service* service);
private:
    static Service* m_service;
    static std::map<std::string, HttpMethod*> m_methods_map;
};