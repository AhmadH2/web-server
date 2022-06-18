#include <iostream>
#include "Server.h"

using namespace boost;
using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
  unsigned short port = std::stoi(argv[1]);
  unsigned short thread_pool_size = std::stoi(argv[2]);
  try
  {

    Server srv;
    srv.start(port, thread_pool_size);
    srv.stop();
  }
  catch(const boost::system::error_code& ec) {
    std::cerr << "Exception: " << ec.message() << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
