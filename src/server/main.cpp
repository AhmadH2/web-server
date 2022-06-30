#include <iostream>
#include "Server.h"
#include <thread>
#include "Exception.h"
using namespace boost;
using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
  try
  {

    if (argc != 2)
    {
      throw myException(ExcepTypes::ARGUMENTS);
    }
    unsigned short port = std::stoi(argv[1]);
    unsigned short thread_pool_size = 5;

    Server srv;
    srv.start(port, thread_pool_size);
    srv.stop();
  }
  catch(myException& myExcep) {
    std::cerr<<myExcep.message();
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
