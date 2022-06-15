#include "Server.h"


Server::Server() {
  m_work.reset(new asio::io_context::work(m_ioc));
}

	// Start the server.
void Server::Start(unsigned short port_num,
  unsigned int thread_pool_size) {
  assert(thread_pool_size > 0);

  // Create and strat Acceptor.
  acc.reset(new Acceptor(m_ioc, port_num));
  acc->Start();
  m_ioc.run();

  // Create specified number of threads and 
  // add them to the pool.
  for (unsigned int i = 0; i < thread_pool_size; i++) {
    boost::thread th(boost::bind(&boost::asio::io_context::run, &m_ioc));
    m_thread_pool.push_back(std::move(th));
  }
}

	// Stop the server.
void Server::Stop() {
  acc->Stop();
  m_ioc.stop();

  for (auto& th : m_thread_pool) {
	th.join();
  }
}
