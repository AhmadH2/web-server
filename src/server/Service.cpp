#include "Service.h"
// #include "httpHead.h"
#include "RequestReader.h"
#include "RequestHandler.h"

const std::map<unsigned int, std::string>
Service::http_status_table =
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
    // read r
    m_requestReader = new RequestReader(this);
    m_requestReader->parseRequest(); // to find the request method & the request resource
}

void Service::onRequestParsed() {
    m_requestHandler = new RequestHandler(m_requestReader, this);
    m_requestHandler->handleRequest();
    return;
}

void Service::onResponseSent(const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
    if (ec) {
        std::cout << "Error occured! at on_response_sent Error code = "
            << ec.value()
            << ". Message: " << ec.message();
            m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);
    }

    m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);

    delete this;
}

std::shared_ptr<boost::asio::ip::tcp::socket> Service::getSocket() {
    return m_sock;
}

std::string Service::getRequestedResource() {
    return m_requested_resource;
}

Service::~Service() {
    m_sock.reset();
}