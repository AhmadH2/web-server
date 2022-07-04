#include <iostream>
#include "Server.h"
#include <thread>
#include "ServerException.h"
using namespace boost;
using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
  Server *srv;
  try
  {

    if (argc != 2)
    {
      throw ServerException(ExceptionTypes::ARGUMENTS);
    }
    unsigned short port = std::stoi(argv[1]);
    unsigned short thread_pool_size = 5;

    srv = new Server();
    srv->start(port, thread_pool_size);
    srv->stop();
  }
  catch(ServerException& svrEx) {
    WrapLog(LogMode::ERROR, svrEx.message());
  }
  catch(const boost::system::error_code& error_code) {
    WrapLog(LogMode::ERROR, error_code.message());
  }
  catch (std::exception& e)
  {
    WrapLog(LogMode::ERROR, e.what());
  }
  catch(...) {
    WrapLog(LogMode::ERROR, "Unknown error");
  }
  
  
  delete srv;
  return 0;
}
