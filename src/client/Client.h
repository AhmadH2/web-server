#pragma once

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <memory>
#include <iostream>
#include "HTTPRequest.h"

using namespace boost;


class HTTPClient {
public:
	HTTPClient();
	std::shared_ptr<HTTPRequest> create_request(std::string host, unsigned short port);
	void close();

private:
	asio::io_context m_ioc;
	std::unique_ptr<asio::io_context::work> m_work;
	std::unique_ptr<boost::thread> m_thread;
};
