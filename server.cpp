
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
  Service(std::shared_ptr<tcp::socket> sock)
    : m_sock(std::move(sock))
  {
  }

  void start()
  {
    do_read();
  }

private:
  void do_read()
  {
    
    m_sock->async_read_some(boost::asio::buffer(data_, max_length),
        [this](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            do_write(length);
          }
        });
  }

  void do_write(std::size_t length)
  {
    std::cout<<data_<<" "<< length<<"\n";
    if(strcmp(data_, "how are you") == 0) {
      char msg[43] ="Im goood";
      std::size_t s= 15; 
      boost::asio::async_write(*m_sock.get(), boost::asio::buffer(msg,s),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            do_read();
          }
        });
    } else
    boost::asio::async_write(*m_sock.get(), boost::asio::buffer(data_, length),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            std::cout<<"read again\n";
            do_read();
          }
        });
  }

  std::shared_ptr<tcp::socket> m_sock;
  enum { max_length = 1024 };
  char data_[max_length];
};

//////////////*******************************************************

class Acceptor {
public:
	Acceptor(asio::io_context& ios, unsigned short port_num) :
		m_ios(ios),
		m_acceptor(m_ios,
		tcp::endpoint(
		asio::ip::address_v4::any(),
		port_num)),
		m_isStopped(false)
	{}

	// Start accepting incoming connection requests.
	void Start() {
		m_acceptor.listen();
		InitAccept();
	}

	// Stop accepting incoming connection requests.
	void Stop() {
		m_isStopped.store(true);
	}

private:
	void InitAccept() {
		std::shared_ptr<tcp::socket>
			sock(new tcp::socket(m_ios));
    std::cout<<"init accept\n ";

		m_acceptor.async_accept(*sock.get(),
			[this, sock](
			const boost::system::error_code& error)
		{
      std::cout<<"handle init accept\n ";
			onAccept(error, sock);
		});
	}

	void onAccept(const boost::system::error_code& ec,
		std::shared_ptr<tcp::socket> sock)
	{
    std::cout<<"ec value: "<<ec.value()<<std::endl;
		if (ec.value() == 0) {
			(new Service(sock))->start();
      //std::make_shared<Service>((*sock.get())))->start();
      std::cout<<"make pointer shared\n";
		}
		else {
			std::cout << "Error occured! Error code = "
				<< ec.value()
				<< ". Message: " << ec.message();
		}

		// Init next async accept operation if
		// acceptor has not been stopped yet.
		if (!m_isStopped.load()) {
			InitAccept();
		}
		else {
			// Stop accepting incoming connections
			// and free allocated resources.
			m_acceptor.close();
		}
	}

private:
	asio::io_context& m_ios;
	tcp::acceptor m_acceptor;
	std::atomic<bool> m_isStopped;
};

//***************************8
class Server {
public:
	Server() {
		m_work.reset(new asio::io_context::work(m_ioc));
	}

	// Start the server.
	void Start(unsigned short port_num,
		unsigned int thread_pool_size) {

		assert(thread_pool_size > 0);

		// Create and strat Acceptor.
		acc.reset(new Acceptor(m_ioc, port_num));
		acc->Start();
    m_ioc.run();
		// Create specified number of threads and 
		// add them to the pool.
		for (unsigned int i = 0; i < thread_pool_size; i++) {
			boost::thread th(boost::bind(&boost::asio::io_context::run, &m_ioc));

			m_thread_pool.push_back(std::move(th));
		}
	}

	// Stop the server.
	void Stop() {
		acc->Stop();
		m_ioc.stop();

		for (auto& th : m_thread_pool) {
			th.join();
		}
	}

private:
	asio::io_context m_ioc;
	std::unique_ptr<asio::io_context::work> m_work;
	std::unique_ptr<Acceptor> acc;
	std::vector<boost::thread> m_thread_pool;
  //std::vector<std::unique_ptr<boost::thread>> m_thread_pool;
};


int main()
{
  unsigned short port_num = 3333;
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
