#include "Service.h"
#include "RequestReader.h"
#include "RequestHandler.h"

const std::map<unsigned int, std::string>
Service::m_http_status_table =
{
	{ 200, "200 OK" },
	{ 404, "404 Not Found" },
	{ 413, "413 Request Entity Too Large" },
	{ 500, "500 Server Error" },
	{ 501, "501 Not Implemented" },
	{ 505, "505 HTTP Version Not Supported" }
};

Service::Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock) :
    m_sock(sock) {};

void Service::start_handling() {
    m_requestReader.reset(new RequestReader(this));
    m_requestReader->parseRequest(); // to find the request method & the request resource
}

void Service::onRequestParsed() {
    m_requestHandler.reset(new RequestHandler(this));
    m_requestHandler->handleRequest();
    return;
}

void Service::onResponseSent(const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
    
    if (ec) {
        WrapLog(LogMode::ERROR, "error accured at sending the respond");
        m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);
        throw ec;
    }

    WrapLog(LogMode::INFO, "Response sent succesfully");
    m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);
    delete this;
}

std::shared_ptr<boost::asio::ip::tcp::socket> Service::getSocket() {
    return m_sock;
}

std::string Service::getRequestedResource() {
    return m_requested_resource;
}

std::string Service::getRequestMethod() {
    return m_request_method;
}

std::string Service::getStatusPhrase(int statusCode) {
    return m_http_status_table.at(statusCode);
}

Service::~Service() {
    m_sock.reset();
    m_requestHandler.reset();
    m_requestReader.reset();
}