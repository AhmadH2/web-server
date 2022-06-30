#include "HTTPRequest.h"

HTTPRequest::HTTPRequest(boost::asio::io_context& ioc, std::string host, unsigned short port):
    m_sock(new asio::ip::tcp::socket(ioc)),
    m_resolver(ioc),
    m_ioc(ioc) {
    m_method = "";
    m_uri = "";
    m_port = port;
    m_host = host;
}

void HTTPRequest::setMethod(std::string method) {
    m_method = method;
}

void HTTPRequest::setURI(std::string uri) {
    m_uri = uri;
}

void HTTPRequest::setBody(std::string body) {
    m_body = body;
}
std::string HTTPRequest::getURI() {
    return m_uri;
}
std::string HTTPRequest::getMethod() {
    return m_method;
}

void HTTPRequest::execute() {
    // Ensure that precorditions hold.
    assert(m_port > 0);
    assert(m_host.length() > 0);
    assert(m_uri.length() > 0);

    // Prepare the resolving query.
    asio::ip::tcp::resolver::query resolver_query(m_host,
        std::to_string(m_port),
        asio::ip::tcp::resolver::query::numeric_service);

    m_resolver.async_resolve(resolver_query,
        [this](const boost::system::error_code& ec,
        asio::ip::tcp::resolver::iterator iterator)
    {
        if(ec) {
            std::cout<<"error in resolving "<<ec.message()<<"\n";
        }
        connect(ec, iterator);
    });
}

void HTTPRequest::connect(const boost::system::error_code& ec,
        asio::ip::tcp::resolver::iterator iterator) {

   if (ec) {
        std::cout<<"error: "<<ec.value();
        throw ec;
	}

    asio::async_connect(*m_sock, iterator,
        [this](const boost::system::error_code& ec,
        asio::ip::tcp::resolver::iterator iterator)
    {
        request(ec, iterator);
    });
}

void HTTPRequest::request(const boost::system::error_code& ec, 
asio::ip::tcp::resolver::iterator iterator) {
    if(ec) throw ec;

    std::string m_request_buf;
    // request status line
    m_request_buf = m_method + " " + m_uri + " HTTP/1.0\r\n";
    
    // request header line
    m_request_buf += "Host: " + m_host + "\r\n";
    m_request_buf += "Sender: Ahmad\r\n";
    m_request_buf += "Type: JSON\r\n";

    // empty line
    m_request_buf += "\r\n";
    // request body
    m_request_buf += m_body + "\r\n";
    // m_request_buf += "{\"readme\":{\"name\":\"txt.txt\"}}\r\n";
    
    // end request to the server
    std::cout<<"Request is: \n" << m_request_buf;
    asio::async_write(*m_sock,
        asio::buffer(m_request_buf),
        [this](const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        response(ec);
    });
}

void HTTPRequest::response(const boost::system::error_code& ec) {
    if(ec) throw ec;
    m_response = new HTTPResponse(m_sock, m_ioc);
    m_response->read_response();
}