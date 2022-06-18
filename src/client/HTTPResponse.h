#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <memory>

using namespace boost;

class HTTPResponse {
public:
    HTTPResponse(std::shared_ptr<asio::ip::tcp::socket> m_sock, asio::io_context& m_ioc);
    void read_response();
private:
    std::shared_ptr<asio::ip::tcp::socket> m_sock;
    asio::io_context &m_ioc;
    asio::streambuf m_response_buf;
    std::istream m_response_stream;
};