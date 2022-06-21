#pragma once
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>

#include <fstream>
#include <atomic>
#include <thread>
#include <iostream>
#include <map>

#include "httpMethod.h"
#include "httpGet.h"
#include "httpHead.h"
#include "httpEcho.h"

using namespace boost;

class Service {
	// friend class HttpMethod;
	// friend class HttpGet;
	

public:
	Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock);
	void start_handling();
	std::string getRequestedResource();
	static const std::map<unsigned int, std::string> http_status_table;
	std::shared_ptr<boost::asio::ip::tcp::socket> getSocket();
	void on_response_sent(const boost::system::error_code& ec,
		std::size_t bytes_transferred);
	void send_response();

private:
	void on_request_line_received(
		const boost::system::error_code& ec,
		std::size_t bytes_transferred);
	
	void on_headers_received(const boost::system::error_code& ec,
		std::size_t bytes_transferred);
	
	void process_request();
  
	
	
	

	// Here we perform the cleanup.
	void on_finish();

private:
	std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
	boost::asio::streambuf m_request_buf;
	std::map<std::string, std::string> m_request_headers;
	std::string m_requested_resource;

	std::unique_ptr<char[]> m_resource_buf;
	unsigned int m_response_status_code;
	std::size_t m_resource_size_bytes;
	std::string m_response_headers;
	std::string m_response_status_line;
	std::string m_request_method;
	HttpMethod* m_http_method;
};



