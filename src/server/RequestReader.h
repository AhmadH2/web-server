#pragma once
#include <boost/asio.hpp>
#include <map>
#include "Service.h"
#include "Logger.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <istream>

class RequestReader {
public:
    void parseRequest();
    // void on_request_line_received(std::size_t bytes_transferred);
	// void on_headers_received(std::size_t bytes_transferred);
    RequestReader(Service* service);
    std::string getRequestMethod();
    std::string getRequestResource();
    // ~RequestReader();
private:
    // void readRequestLine();
    void readRequestHeader();
    void parseRequestLine();
    void parseRequestHeader();
    void readRequestBody();
    void parseRequestBody();
private:
    Service* m_service;
	boost::asio::streambuf m_request_buf;
    boost::asio::streambuf m_request_body_buf;
	std::map<std::string, std::string> m_request_headers;
	std::string m_requested_resource;
    std::string m_request_method;
    int m_response_status_code;
};