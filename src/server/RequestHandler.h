#pragma once
#include "httpMethod.h"
#include "MethodFactory.h"
#include "Service.h"

class RequestReader;

class RequestHandler {
public:
    RequestHandler(Service* service);
    void handleRequest();
    void sendResponse();
private:
    HttpMethod* m_http_method;
	MethodFactory* m_methodFactory;
    Service* m_service;
    unsigned int m_response_status_code;
    std::string m_response_status_line;
    std::string m_response_headers;
};