#include "Client.h"

HTTPClient::HTTPClient(){
    m_work.reset(new boost::asio::io_context::work(m_ioc));
    m_thread.reset(new boost::thread(boost::bind(&boost::asio::io_context::run, &m_ioc)));
    
}

std::shared_ptr<HTTPRequest> HTTPClient::create_request(std::string host, unsigned short port)
{
    return std::shared_ptr<HTTPRequest>(
        new HTTPRequest(m_ioc, host, port));
}

void HTTPClient::close() {
    // Destroy the work object. 
    m_work.reset(NULL);

    // Waiting for the I/O thread to exit.
    m_thread->join();
}

