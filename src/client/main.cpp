
// #include <cstdlib>
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include "Client.h"
#include "HTTPRequest.h"


int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      throw "Not suatable arguments list\n";
    }

    std::string host = argv[1];
    unsigned short port = std::stoi(argv[2]);
    std::string method;
    std::string uri;

    HTTPClient client;
		std::shared_ptr<HTTPRequest> req = client.create_request(host, port);

    std::cout<<"ENTER HTTP method: ";
    std::cin>>method;
    std::cout<<"ENTER file URI start with / ";
    std::cin>>uri;

    req->setMethod(method);
    req->setURI(uri);
    req->execute();

    std::this_thread::sleep_for(std::chrono::seconds(15));
    client.close();

  }
  catch(boost::system::error_code& ec) {
    std::cerr<< "ERROR CODE: "<<ec.message();
  }
  catch (char const* e)
  {
    std::cerr << "Exception: " << e << "\n";
  }

  return 0;
}
