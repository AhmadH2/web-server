#include "httpEcho.h"
#include "Service.h"

HttpEcho::HttpEcho(Service* service): HttpMethod(service) {}

void HttpEcho::processRequest() {}

void HttpEcho::sendResponse() {

    m_response_status_line = std::string("HTTP/1.0 ") +
        m_service->getStatusPhrase(m_response_status_code) + "\r\n";

    m_response_headers += "\r\n";

    std::vector<asio::const_buffer> response_buffers;
    response_buffers.push_back(
        asio::buffer(m_service->getRequestedResource()));

    if (m_response_headers.length() > 0) {
        response_buffers.push_back(
            asio::buffer(m_response_headers));
    }

    asio::async_write(*(m_service->getSocket()),
        response_buffers,
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        this->m_service->onResponseSent(ec, bytes_transferred);
        return;
    });
}