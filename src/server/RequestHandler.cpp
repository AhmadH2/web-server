#include "RequestHandler.h"
#include "RequestReader.h"

RequestHandler::RequestHandler(RequestReader* requestReader, Service* service)
: m_requestReader(requestReader), m_service(service) {}

void RequestHandler::handleRequest() {
    m_methodFactory = new MethodFactory(m_service);

    m_http_method = m_methodFactory->getMethod(m_requestReader->getRequestMethod());
    if(m_http_method == nullptr) {
        m_response_status_code = 501;
        sendResponse();
        return;
    }
    m_http_method->processRequest();
    m_http_method->sendResponse();

    return;
}


void RequestHandler::sendResponse()  {

    auto status_line =
        m_service->http_status_table.at(m_response_status_code);

    m_response_status_line = std::string("HTTP/1.0 ") +
        status_line +
        "\r\n";

    m_response_headers += "\r\n";


    std::vector<asio::const_buffer> response_buffers;
    response_buffers.push_back(
        asio::buffer(m_response_status_line));

    if (m_response_headers.length() > 0) {
        response_buffers.push_back(
            asio::buffer(m_response_headers));
    }

    // Initiate asynchronous write operation.
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
