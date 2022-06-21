#include "httpEcho.h"
#include "Service.h"

HttpEcho::HttpEcho(Service* service): HttpMethod(service) {}

void HttpEcho::processRequest() {}

void HttpEcho::sendResponse() {
    auto status_line = m_service->http_status_table.at(m_response_status_code);

    m_response_status_line = std::string("HTTP/1.0 ") +
        status_line +
        "\r\n";

    m_response_headers += "\r\n";

    std::vector<asio::const_buffer> response_buffers;
    response_buffers.push_back(
        asio::buffer(m_service->getRequestedResource()));

    if (m_response_headers.length() > 0) {
        response_buffers.push_back(
            asio::buffer(m_response_headers));
    }

    // Initiate asynchronous write operation.
    asio::async_write(*(m_service->getSocket()),
        response_buffers,
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        this->m_service->on_response_sent(ec, bytes_transferred);
        return;
    });
}