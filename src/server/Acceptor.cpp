
#include "Acceptor.h"
#include "Service.h"

Acceptor::Acceptor(asio::io_context& ioc, unsigned short port) :
  m_ioc(ioc),
  m_acceptor(m_ioc, tcp::endpoint(asio::ip::address_v4::any(), port)),
  m_sock(new tcp::socket(m_ioc)),
  m_isStopped(false)
{}

// Start accepting connection requests.
void Acceptor::start() {
  m_acceptor.listen();
  InitAccept();
}

// Stop accepting connection requests.
void Acceptor::stop() {
  m_isStopped.store(true);
}

// Establish a connection with the client
void Acceptor::InitAccept() {
  std::cout<<"init accept\n ";

  // A passive socket listens for incomming connection requests
  m_acceptor.async_accept(*m_sock,
	[this](
		const boost::system::error_code& ec){
		  try{
			if(ec) 
				throw ec;
			onAccept(ec);
		  } 
		  catch(boost::system::error_code& ec) {
			std::cout << "Error occured! Error code = "
			<< ec.value()
			<< ".Error Message: " << ec.message();
		  }
		}
	);
}

// start handling request
void Acceptor::onAccept(const boost::system::error_code& ec) {
	if (!ec) {
		// void start_handling();
		(new Service(m_sock))->start_handling();
	}
	else 
	    throw ec;

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