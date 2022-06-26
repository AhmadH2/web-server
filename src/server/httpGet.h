#pragma once
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <boost/filesystem.hpp>
#include <memory>
#include "httpMethod.h"
#include <map>
#include <boost/filesystem.hpp>
#include <fstream>
#include "httpMethod.h"

using namespace boost;

class Service;

class HttpGet: public HttpMethod {
public:
    HttpGet(Service* service);
    void processRequest();
    void sendResponse();
    void setRequestedRes(std::string res);
    virtual ~HttpGet();

private:
    std::size_t m_resource_size_bytes;
	std::unique_ptr<char[]> m_resource_buf;
};