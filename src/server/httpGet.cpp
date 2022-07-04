#include "httpGet.h"
#include "Service.h"

HttpGet::HttpGet(Service* service): HttpMethod(service) {
}

void HttpGet::processRequest() {
    std::string resource_file_path = "." + m_service->getRequestedResource();

    if (!boost::filesystem::exists(resource_file_path)) {
        m_response_status_code = 404;
        WrapLog(LogMode::ERROR, "File not found");
        return;
    }

    std::ifstream resource_fstream(resource_file_path, std::ifstream::binary);

    if (!resource_fstream.is_open()) {
        m_response_status_code = 500;
        return;
    }

    // Find out file size of the file.
    resource_fstream.seekg(0, std::ifstream::end);
    m_resource_size_bytes = static_cast<std::size_t>(resource_fstream.tellg());
    resource_fstream.seekg(std::ifstream::beg);

    m_resource_buf.reset(new char[m_resource_size_bytes]);
    
    resource_fstream.read(m_resource_buf.get(), m_resource_size_bytes);

    m_response_headers += std::string("content-length") +
        ": " + std::to_string(m_resource_size_bytes) + "\r\n";
}

void HttpGet::sendResponse()  {

    m_response_status_line = std::string("HTTP/1.0 ") +
        m_service->getStatusPhrase(m_response_status_code) + "\r\n";

    m_response_headers += "\r\n";

    std::vector<asio::const_buffer> response_buffers;
    response_buffers.push_back(
        asio::buffer(m_response_status_line));

    if (m_response_headers.length() > 0) {
        response_buffers.push_back(
            asio::buffer(m_response_headers));
    }

    if (m_resource_size_bytes > 0) {
        response_buffers.push_back(
            asio::buffer(m_resource_buf.get(),
            m_resource_size_bytes));
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

HttpGet::~HttpGet() {
    m_resource_buf.reset();
}