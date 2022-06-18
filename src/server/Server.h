#pragma once
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "Acceptor.h"

using namespace boost;
using boost::asio::ip::tcp;

// represents the server
// creates instance of Acceptor class to handle requests
class Server {
public:
  Server();
  // Start the server.
  void start(unsigned short port,
	unsigned int thread_pool_size);
  // Stop the server.
  void stop();

private:
  asio::io_context m_ioc;
  std::unique_ptr<asio::io_context::work> m_work;
  std::unique_ptr<Acceptor> acceptor;
  std::vector<boost::thread> m_thread_pool;
};

