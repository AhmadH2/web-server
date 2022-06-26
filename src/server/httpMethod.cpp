#include "httpMethod.h"

HttpMethod::HttpMethod(Service* service): m_service(service) {
    m_response_status_code = 200;
    m_response_headers = "";
    m_response_status_line = "";
}


HttpMethod::~HttpMethod() {}