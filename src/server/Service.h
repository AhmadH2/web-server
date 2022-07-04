#pragma once
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <atomic>
#include <thread>
#include <iostream>
#include <map>
#include "httpMethod.h"
#include "MethodFactory.h"
#include "Logger.h"

using namespace boost;

class RequestReader;
class RequestHandler;

class Service {
friend class RequestReader;
public:
	Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock);
	void start_handling();
	void onRequestParsed();
	std::string getRequestedResource();
	std::string getRequestMethod();
	std::shared_ptr<boost::asio::ip::tcp::socket> getSocket();
	void onResponseSent(const boost::system::error_code& ec,
		std::size_t bytes_transferred);
	std::string getStatusPhrase(int statusCode);
	~Service();

private:
	std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
	std::string m_requested_resource;
	std::string m_request_method;
	std::unique_ptr<RequestReader> m_requestReader;
	std::unique_ptr<RequestHandler> m_requestHandler;
	static const std::map<unsigned int, std::string> m_http_status_table;
};



