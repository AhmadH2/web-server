#pragma once
#include "httpMethod.h"
#include "httpGet.h"
#include "httpHead.h"
#include "httpEcho.h"
#include <boost/algorithm/string.hpp>
#include <string>
#include "ServerException.h"
#include "Service.h"

class MethodFactory {
    
public:
    static HttpMethod* getMethod(Service* service);
private:
    static std::map<std::string, HttpMethod*> m_methods_map;
};