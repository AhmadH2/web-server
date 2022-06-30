#pragma once
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "Acceptor.h"

using namespace boost;
using boost::asio::ip::tcp;



/** 
 * @brief Server class used for creating and managing server.
 * @author Ahmad Horyzat
 *  @date Jun 2022

    Detailed description follows here.
  
    */
class Server {
public:
/** Default constructor. Does something.
        */
  Server();
  /**Start the server
        */
  void start(unsigned short port,
	unsigned int thread_pool_size);
  /** Stop the server*/
  void stop();

private:
  asio::io_context m_ioc; /**<initial io_context*/
  std::unique_ptr<asio::io_context::work> m_work; /**<initial worker*/
  std::unique_ptr<Acceptor> acceptor; /**<instance of Acceptro class to accept connections*/
  std::vector<boost::thread> m_thread_pool; /**<Vector to store created threads*/
};

