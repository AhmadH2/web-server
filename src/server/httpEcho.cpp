#include "httpEcho.h"
#include "Service.h"

HttpEcho::HttpEcho(Service* service): HttpMethod(service) {}

void HttpEcho::processRequest() {
    // std::cout<<"requested resource: "<<m_service->getRequestedResource();
    // std::string resource_file_path = "." + m_service->getRequestedResource();
    //     // m_requested_resource;

    // std::cout<<"Resource path"<<resource_file_path<<"\n";
    // if (!boost::filesystem::exists(resource_file_path)) {
    //     // Resource not found.
    //     m_response_status_code = 404;
    //     std::cout<<"file not exist\n";
    //     m_service->send_response();
    //     // m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);
    //     return;
    // }

    // std::ifstream resource_fstream(
    //     resource_file_path,
    //     std::ifstream::binary);

    // if (!resource_fstream.is_open()) {
    //     // Could not open file. 
    //     // Something bad has happened.
    //     m_response_status_code = 500;
    //     // m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);
    //     m_service->send_response();
    //     return;
    // }

    // // Find out file size.
    // resource_fstream.seekg(0, std::ifstream::end);
    // m_resource_size_bytes =
    //     static_cast<std::size_t>(
    //     resource_fstream.tellg());
    
    //  m_response_headers += std::string("content-length") +
    //     ": " +
    //     std::to_string(m_resource_size_bytes) +
    //     "\r\n";
    // std::cout<<"response header: "<<"\n";

}

void HttpEcho::sendResponse() {
    auto status_line = m_service->http_status_table.at(m_response_status_code);
        // http_status_table.at(m_response_status_code);

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

    // if (m_resource_size_bytes > 0) {
    //     response_buffers.push_back(
    //         asio::buffer(m_resource_buf.get(),
    //         m_resource_size_bytes));
    // }

    // Initiate asynchronous write operation.
    asio::async_write(*(m_service->getSocket()),
        response_buffers,
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        // m_service->on_response_sent(ec,
        //     bytes_transferred);
        this->m_service->on_response_sent(ec, bytes_transferred);
        // m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);
        return;
    });
}