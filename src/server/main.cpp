#include <iostream>
#include "Server.h"
#include <thread>

using namespace boost;
using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
  unsigned short port = std::stoi(argv[1]);
  unsigned short thread_pool_size = std::stoi(argv[2]);
  try
  {

     if (argc != 3)
    {
      throw "Not suatable arguments list\n";
    }

    Server srv;
    srv.start(port, thread_pool_size);

    srv.stop();
  }
  catch(const boost::system::error_code& ec) {
    std::cerr << "error code Exception: " << ec.message() << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  catch(...) {
    std::cerr << "Unknown error\n";
  }
  return 0;
}
