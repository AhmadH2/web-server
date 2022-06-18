#include "Service.h"

const std::map<unsigned int, std::string>
Service::http_status_table =
{
	{ 200, "200 OK" },
	{ 404, "404 Not Found" },
	{ 413, "413 Request Entity Too Large" },
	{ 500, "500 Server Error" },
	{ 501, "501 Not Implemented" },
	{ 505, "505 HTTP Version Not Supported" }
};

Service::Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock) :
    m_sock(sock),
    m_request_buf(4096),
    m_response_status_code(200), // initialized to assume success.
    m_resource_size_bytes(0)
{};

void Service::start_handling() {
    asio::async_read_until(*m_sock.get(),
        m_request_buf,
        "\r\n",
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        on_request_line_received(ec, bytes_transferred);
    });
}


void Service::on_request_line_received(
    const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
    if (ec) {
        std::cout << "Error occured! at on_request_line_received Error code = "
            << ec.value()
            << ". Message: " << ec.message();

        if (ec == asio::error::not_found) {
            // No delimiter has been fonud in the
            // request message.

            m_response_status_code = 413;
            send_response();

            return;
        }
        else {
            // In case of any other error � 
            // close the socket and clean up.
            on_finish();
            return;
        }
    }

    // Parse the request line.
    std::string request_line;
    std::istream request_stream(&m_request_buf);
    std::getline(request_stream, request_line, '\r');
    // Remove symbol '\n' from the buffer.
    request_stream.get();

    // Parse the request line.
    std::string request_method;
    std::istringstream request_line_stream(request_line);
    request_line_stream >> request_method;
    std::cout<<"method: " <<request_method<<"\n";

    // Need to be refactored to be open for extensions, closed for modifications
    // following factory design pattern
    // if(request_method == "get") {
    
    // }
    // else if(request_method == "head") {

    // }
    // else if(request_method == "echo") {

    // }
    // else {
    //             // Unsupported method.
    //     m_response_status_code = 501;
    //     send_response();

    //     return;
    // }

    if (request_method.compare("get") != 0) {
        // Unsupported method.
        m_response_status_code = 501;
        send_response();

        return;
    }

    request_line_stream >> m_requested_resource;

    std::string request_http_version;
    request_line_stream >> request_http_version;
    std::cout<<"resource: "<<m_requested_resource<<"\n";
    if (request_http_version.compare("HTTP/1.0") != 0) {
        // Unsupported HTTP version or bad request.
        m_response_status_code = 505;
        send_response();

        return;
    }
    // At this point the request line is successfully
    // received and parsed

    // Read the request headers.
    asio::async_read_until(*m_sock.get(),
        m_request_buf,
        "\r\n\r\n",
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        on_headers_received(ec,
            bytes_transferred);
    });
    

    return;
}

void Service::on_headers_received(const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
    if (ec) {
        std::cout << "Error occured! at on_headers_received Error code = "
            << ec.value()
            << ". Message: " << ec.message();

        if (ec == asio::error::not_found) {
            // No delimiter has been fonud in the
            // request message.

            m_response_status_code = 413;
            send_response();
            return;
        }
        else {
            // In case of any other error - close the
            // socket and clean up.
            on_finish();
            return;
        }
    }

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

    // Now we have all we need to process the request.
    process_request();
    send_response();

    return;
}

void Service::process_request() {
    std::cout<<"processing reques\n";
    // Read file.
    std::string resource_file_path = "." +
        m_requested_resource;

    std::cout<<"Resource path"<<resource_file_path<<"\n";
    if (!boost::filesystem::exists(resource_file_path)) {
        // Resource not found.
        m_response_status_code = 404;
        std::cout<<"file not exist\n";
        return;
    }

    std::ifstream resource_fstream(
        resource_file_path,
        std::ifstream::binary);

    if (!resource_fstream.is_open()) {
        // Could not open file. 
        // Something bad has happened.
        m_response_status_code = 500;

        return;
    }

    // Find out file size.
    resource_fstream.seekg(0, std::ifstream::end);
    m_resource_size_bytes =
        static_cast<std::size_t>(
        resource_fstream.tellg());

    m_resource_buf.reset(
        new char[m_resource_size_bytes]);

    resource_fstream.seekg(std::ifstream::beg);
    resource_fstream.read(m_resource_buf.get(),
        m_resource_size_bytes);

    m_response_headers += std::string("content-length") +
        ": " +
        std::to_string(m_resource_size_bytes) +
        "\r\n";
        std::cout<<"end processing request\n";
    // resource_fstream.close();
    // boost::filesystem::complete(resource_file_path);
}

void Service::send_response()  {
    std::cout<<"send response\n";
    m_sock->shutdown(
        asio::ip::tcp::socket::shutdown_receive);

    auto status_line =
        http_status_table.at(m_response_status_code);

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

    if (m_resource_size_bytes > 0) {
        response_buffers.push_back(
            asio::buffer(m_resource_buf.get(),
            m_resource_size_bytes));
    }

    // Initiate asynchronous write operation.
    asio::async_write(*m_sock.get(),
        response_buffers,
        [this](
        const boost::system::error_code& ec,
        std::size_t bytes_transferred)
    {
        on_response_sent(ec,
            bytes_transferred);
    });
}

void Service::on_response_sent(const boost::system::error_code& ec,
    std::size_t bytes_transferred)
{
    if (ec) {
        std::cout << "Error occured! at on_response_sent Error code = "
            << ec.value()
            << ". Message: " << ec.message();
    }

    m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);

    on_finish();
}

// Here we perform the cleanup.
void Service::on_finish() {
    delete this;
}

