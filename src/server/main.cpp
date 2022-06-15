
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <atomic>
#include "Server.h"

using namespace boost;
using boost::asio::ip::tcp;

//////////////*******************************************************

//***************************8

int main()
{
  unsigned short port_num = 33;
  unsigned short thread_pool_size = 5;
  try
  {

    //boost::asio::io_context io_context;

    Server srv;

    srv.Start(port_num, thread_pool_size);
    //boost::thread t(boost::bind(&boost::asio::io_context::run, &io_context));
    //std::cout<<"Between threads:\n";
    // io_context.run();
    std::cout<<"After io.run()\n";
    //t.join();
    //io_context.run();
    srv.Stop();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
