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
#include "Acceptor.h"

using namespace boost;
using boost::asio::ip::tcp;


class Server {
public:
  Server();
  // Start the server.
  void Start(unsigned short port_num,
	unsigned int thread_pool_size);
  // Stop the server.
  void Stop();

private:
  asio::io_context m_ioc;
  std::unique_ptr<asio::io_context::work> m_work;
  std::unique_ptr<Acceptor> acc;
  std::vector<boost::thread> m_thread_pool;
  //std::vector<std::unique_ptr<boost::thread>> m_thread_pool;
};

