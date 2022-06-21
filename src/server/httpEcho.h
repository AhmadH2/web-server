#pragma once
#include "httpMethod.h"

class HttpEcho: public HttpMethod {
public:
    HttpEcho(Service* service);
    void processRequest();
    void sendResponse();
private:
    std::size_t m_resource_size_bytes;
    // std::unique_ptr<char[]> m_resource_buf;
};