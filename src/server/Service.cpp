#include "Service.h"
#include<string>

Service::Service(std::shared_ptr<tcp::socket> sock)
    : m_sock(std::move(sock))
  {
  }

void Service::start() {
  do_read();
}


void Service::do_read() {
    
  m_sock->async_read_some(boost::asio::buffer(m_data, max_length),
    [this](boost::system::error_code ec, std::size_t length)
    {
      if (!ec) {
        do_write(length);
      }
  });
}

void Service::do_write(std::size_t length) {
  std::cout<<m_data<<" "<< length<<"\n";
  if(strcmp(m_data, "how are you") == 0) {
    std::string msg ="Im goood";
    std::size_t s = msg.length(); 
    boost::asio::async_write(*m_sock.get(), boost::asio::buffer(msg,s),
    [this](boost::system::error_code ec, std::size_t /*length*/)
    {
        if (!ec)
        {
        do_read();
        }
    });
  } else
    boost::asio::async_write(*m_sock.get(), boost::asio::buffer(m_data, length),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            std::cout<<"read again\n";
            do_read();
          }
        });
  }
