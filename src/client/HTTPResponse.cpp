#include "HTTPResponse.h"

HTTPResponse::HTTPResponse(std::shared_ptr<asio::ip::tcp::socket> sock,
    asio::io_context &ioc): m_sock(sock), m_ioc(ioc), m_response_stream(&m_response_buf) {}

void HTTPResponse::read_response() {

    
    asio::async_read(*m_sock, m_response_buf,
    [this](const boost::system::error_code& ec, std::size_t bytes_transferred) {
        if(!ec || ec == boost::asio::error::eof)
            std::cout << "Response: \n"<<m_response_stream.rdbuf()<<"\n";
        else 
            throw ec.message();
    });

}