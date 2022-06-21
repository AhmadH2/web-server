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

    // HttpGet(std::string requested_esource, 
    // std::shared_ptr<boost::asio::ip::tcp::socket> sock,
    // const std::map<unsigned int, std::string> http_status_table);
    void processRequest();
    void sendResponse();
    void setRequestedRes(std::string res);

private:
    std::size_t m_resource_size_bytes;
    // std::string m_requested_resource;
	std::unique_ptr<char[]> m_resource_buf;
    // std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    // const std::map<unsigned int, std::string> m_http_status_table;
    // std::shared_ptr<Service> m_service;

    // int m_response_status_code;
    // std::string m_response_headers;
    // std::string m_response_status_line;
};