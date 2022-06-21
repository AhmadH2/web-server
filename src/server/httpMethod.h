#pragma once
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <atomic>
#include <thread>
#include <iostream>
#include <map>
#include <memory>

class Service;
class HttpMethod
{
public:
    HttpMethod(Service* service);
    void virtual processRequest();
    void virtual sendResponse();

protected:
    Service* m_service;
    int m_response_status_code;
    std::string m_response_headers;
    std::string m_response_status_line;
private:
  
};


