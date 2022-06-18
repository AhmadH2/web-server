#pragma once
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <atomic>

using namespace boost;
using boost::asio::ip::tcp;

// Responsible for accepting connection requests from clients 
// and create instance of Service class for each request to handle the request
class Acceptor {
public:
  Acceptor(asio::io_context& ioc, unsigned short port);

	// Start accepting connection requests.
  void start();

	// Stop accepting connection requests.
  void stop();

private:
  void InitAccept();

  void onAccept(const boost::system::error_code& ec);

private:
	asio::io_context& m_ioc;
	tcp::acceptor m_acceptor;
	std::shared_ptr<tcp::socket> m_sock;
	std::atomic<bool> m_isStopped;
};
