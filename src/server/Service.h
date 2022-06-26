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
	std::shared_ptr<boost::asio::ip::tcp::socket> getSocket();
	void onResponseSent(const boost::system::error_code& ec,
		std::size_t bytes_transferred);
	static const std::map<unsigned int, std::string> http_status_table;
	~Service();

private:
	std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
	std::string m_requested_resource;
	std::string m_request_method;
	RequestReader* m_requestReader;
	RequestHandler* m_requestHandler;
};



