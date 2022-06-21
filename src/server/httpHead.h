#pragma once
#include "httpMethod.h"

class HttpHead: public HttpMethod {
public:
    HttpHead(Service* service);
    void processRequest();
    void sendResponse();
private:
    std::size_t m_resource_size_bytes;
};