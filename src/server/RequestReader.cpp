#include "RequestReader.h"
#include "WrapLogger.h"

RequestReader::RequestReader(Service* service): m_service(service) {
}

void RequestReader::parseRequest() {

    // read requst 
    asio::async_read_until(*m_service->getSocket(),
        m_request_buf,
        "\r\n",
        [this](
        const boost::system::error_code& ec, std::size_t bytes_transferred)
        {
            if(!ec) {
                parseRequestLine();
                return;
            }

            if (ec == asio::error::not_found) {
                // No delimiter has been fonud in the request message.
                m_response_status_code = 413;
                return;
            }
            else {
                delete this;
                return;
            }
        });
    return;
}

void RequestReader::parseRequestLine ()
{
    std::string request_line;
    std::istream request_stream(&m_request_buf);
    std::getline(request_stream, request_line, '\r');
    // Remove symbol '\n' from the buffer.
    request_stream.get();

    std::istringstream request_line_stream(request_line);
    request_line_stream >> m_request_method;
    request_line_stream >> m_requested_resource;
    m_service->m_request_method = m_request_method;
    m_service->m_requested_resource = m_requested_resource;

    std::string request_http_version;
    request_line_stream >> request_http_version;
    if (request_http_version.compare("HTTP/1.0") != 0) {
        // Unsupported HTTP version or bad request.
        m_response_status_code = 505;
        return;
    }

    parseRequestHeader();
    return;
}

void RequestReader::parseRequestHeader()
{
    std::string request_head;
    std::istream request_stream(&m_request_buf);
    std::string header_name, header_value;

    std::getline(request_stream, request_head, '\r');
    request_stream.get(); 
    while(!request_head.empty()) {
        std::istringstream request_header_stream(request_head);
        request_header_stream >> header_name;
        request_header_stream >> header_value;
        header_name = header_name.substr(0, header_name.length()-1); // remove ':' from header_name
        m_request_headers[header_name] = header_value;
        std::getline(request_stream, request_head, '\r'); // read next header
        request_stream.get();
    }

    parseRequestBody();
    return;
}


void RequestReader::parseRequestBody() {
    std::istream request_stream(&m_request_buf);
    
    try {
        property_tree::ptree tree;
        property_tree::read_json(request_stream, tree);
        if(tree.get<std::string>("read.name") != "") {
            m_requested_resource = "/" + tree.get<std::string>("read.name");
            m_request_method = "get";
            m_service->m_request_method = m_request_method;
            m_service->m_requested_resource = m_requested_resource;
            m_service->onRequestParsed();
            return;
        }
    }
    catch (... ) {
        WrapLog(LogMode::ERROR, "Exception with property tree");
    }

    WrapLog(LogMode::INFO, "Request parsed succesfully");
    m_service->onRequestParsed();
}

std::string RequestReader::getRequestMethod() {
    return this->m_request_method;
}

std::string RequestReader::getRequestResource() {
    return this->m_requested_resource;
}

RequestReader::~RequestReader() {}