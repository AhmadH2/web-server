#include "RequestReader.h"

RequestReader::RequestReader(Service* service): m_service(service) {}

void RequestReader::parseRequest() {
    // read requst line
    asio::async_read_until(*m_service->getSocket(),
        m_request_buf,
        "\r\n",
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        if(!ec) {
            // readRequestHeader();
            parseRequestLine();
            return;
        }

        std::cout << "Error occured! at on_request_line_received Error code = "
            << ec.value()
            << ". Message: " << ec.message();

        if (ec == asio::error::not_found) {
            // No delimiter has been fonud in the
            // request message.

            m_response_status_code = 413;

            return;
        }
        else {
            // In case of any other error 
            // close the socket and clean up.
            // this->~Service();
            // on_finish();
            delete this;

            return;
        }
        
    });
    
}

void RequestReader::parseRequestLine ()
{

    // Parse the request line.
    std::string request_line;
    std::istream request_stream(&m_request_buf);
    std::getline(request_stream, request_line, '\r');

    Logger::getInstance();

    WrapLog(LogMode::INFO, request_line);

    // Remove symbol '\n' from the buffer.
    request_stream.get();

    // Parse the request line.
    std::istringstream request_line_stream(request_line);
    request_line_stream >> m_request_method;
    std::cout<<"method: " <<m_request_method<<"\n";

    request_line_stream >> m_requested_resource;

    std::string request_http_version;
    request_line_stream >> request_http_version;
    std::cout<<"resource: "<<m_requested_resource<<"\n";
    if (request_http_version.compare("HTTP/1.0") != 0) {
        // Unsupported HTTP version or bad request.
        m_response_status_code = 505;

        return;
    }
    // At this point the request line is successfully
    // received and parsed

    readRequestHeader();
    
    return;
}
void RequestReader::readRequestHeader() {
    // Read the request headers.
    asio::async_read_until(*m_service->getSocket(),
        m_request_buf,
        "\r\n\r\n",
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        if(!ec) {
            parseRequestHeader();
            return;
        }

        std::cout << "Error occured! at on_headers_received Error code = "
            << ec.value()
            << ". Message: " << ec.message();

        if (ec == asio::error::not_found) {
            // No delimiter has been fonud in the
            // request message.

            m_response_status_code = 413;
            return;
        }
        else {
            delete this;
            return;
        }

    });
}

void RequestReader::parseRequestHeader()
{

    // Parse and store headers.
    std::istream request_stream(&m_request_buf);
    std::string header_name, header_value;

    while (!request_stream.eof()) {
        std::getline(request_stream, header_name, ':');
        if (!request_stream.eof()) {
            std::getline(request_stream,
                header_value,
                '\r');

            // Remove symbol \n from the stream.
            request_stream.get();
            m_request_headers[header_name] =
                header_value;
        }
    }
    m_service->m_request_method = m_request_method;
    m_service->m_requested_resource = m_requested_resource;
    // m_service->setRequestMethod(m_request_method);
    // m_service->setRequestedResource(m_requested_resource);

    Logger::getInstance();
    WrapLog(LogMode::INFO, "Request parsed succesfully");
    m_service->onRequestParsed();
}

std::string RequestReader::getRequestMethod() {
    return this->m_request_method;
}

std::string RequestReader::getRequestResource() {
    return this->m_requested_resource;
}