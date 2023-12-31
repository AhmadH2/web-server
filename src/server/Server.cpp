#include "Server.h"
#include "WrapLogger.h"

Server::Server() {
  m_work.reset(new asio::io_context::work(m_ioc));
  WrapLogger::getLogger();
}

void Server::start(unsigned short port,unsigned int thread_pool_size) {
  if(port < 0 || thread_pool_size < 0)
    throw ServerException(ExceptionTypes::ARGUMENTS);

  WrapLog(LogMode::TRACE, "Server start");
  // Create and strat Acceptor.
  acceptor.reset(new Acceptor(m_ioc, port));
  acceptor->start();
  
  m_ioc.run();

  // Create specified number of threads and add them to the threads pool.
  for (unsigned int i = 0; i < thread_pool_size; i++) {
    boost::thread th(boost::bind(&boost::asio::io_context::run, &m_ioc));
    m_thread_pool.push_back(std::move(th));
  }
}

void Server::stop() {
  acceptor->stop();
  m_ioc.stop();

  for (auto& th : m_thread_pool) {
	  th.join();
  }
}
