#pragma once
#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include "HTTPResponse.h"

using namespace boost;

class HTTPRequest {
public:
    HTTPRequest(boost::asio::io_context& ioc, std::string host, unsigned short port);
    void setURI(std::string uri);
    void setMethod(std::string method);
    void setBody(std::string body);
    void execute();
    std::string getURI();
    std::string getMethod();
private:
    void connect(const boost::system::error_code& ec,
        asio::ip::tcp::resolver::iterator iterator);
    void request(const boost::system::error_code& ec,
        asio::ip::tcp::resolver::iterator iterator);
    void response(const boost::system::error_code& ec);

private:
    std::string m_host;
    unsigned short m_port;
    std::string m_method;
    std::string m_uri;
    std::string m_body;
    std::shared_ptr<asio::ip::tcp::socket> m_sock;
    asio::ip::tcp::resolver m_resolver;
    asio::io_context& m_ioc;
    HTTPResponse *m_response;
};