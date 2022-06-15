#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <atomic>

using namespace boost;
using boost::asio::ip::tcp;


class Acceptor {
public:
  Acceptor(asio::io_context& ios, unsigned short port_num);
	// Start accepting incoming connection requests.
  void Start();

	// Stop accepting incoming connection requests.
  void Stop();

private:
  void InitAccept();

  void onAccept(const boost::system::error_code& ec,
	std::shared_ptr<tcp::socket> sock);

private:
	asio::io_context& m_ios;
	tcp::acceptor m_acceptor;
	std::atomic<bool> m_isStopped;
};
