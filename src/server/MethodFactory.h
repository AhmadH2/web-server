#pragma once
#include "httpMethod.h"
#include "httpGet.h"
#include "httpHead.h"
#include "httpEcho.h"
#include <boost/algorithm/string.hpp>
#include <string>

class MethodFactory {
    
public:
    HttpMethod* getMethod(std::string method);
    MethodFactory(Service* service);
private:
    Service* m_service;
    std::map<std::string, HttpMethod*> m_methods_map;
};