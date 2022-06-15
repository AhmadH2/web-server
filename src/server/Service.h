#ifndef _service
#define _service


#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <atomic>

using namespace boost;
using boost::asio::ip::tcp;

class Service {
public:
  Service(std::shared_ptr<tcp::socket> sock);
  void start();

private:
  void do_read();
  void do_write(std::size_t length);
  std::shared_ptr<tcp::socket> m_sock;
  static const int max_length = 1024;
  char m_data[max_length];
};

#endif