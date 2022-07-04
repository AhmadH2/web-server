#include "RequestHandler.h"
#include "RequestReader.h"

RequestHandler::RequestHandler(Service* service)
: m_service(service) {}

void RequestHandler::handleRequest() {
    try {
        m_http_method = MethodFactory::getMethod(m_service);
        m_http_method->processRequest();
        m_http_method->sendResponse();
    }
    catch (ServerException& svrEx) {
        WrapLog(LogMode::ERROR, svrEx.message());
        m_response_status_code = 501;
        sendResponse();
    }
    return;
}

void RequestHandler::sendResponse()  {
    m_response_status_line = std::string("HTTP/1.0 ") +
       m_service->getStatusPhrase(m_response_status_code) +
        "\r\n";
 
    m_response_headers += "\r\n";

    std::vector<asio::const_buffer> response_buffers;
    response_buffers.push_back(
        asio::buffer(m_response_status_line));

    if (m_response_headers.length() > 0) {
        response_buffers.push_back(
            asio::buffer(m_response_headers));
    }

    asio::async_write(*m_service->getSocket(),
        response_buffers,
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        m_service->onResponseSent(ec,
            bytes_transferred);
    });
}
