
#include "Acceptor.h"
#include "Service.h"

Acceptor::Acceptor(asio::io_context& ioc, unsigned short port) :
  m_ioc(ioc),
  m_acceptor(m_ioc, tcp::endpoint(asio::ip::address_v4::any(), port)),
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
  std::cout<<"init accept\n";
  std::shared_ptr<asio::ip::tcp::socket>
			sock(new asio::ip::tcp::socket(m_ioc));

//   m_sock->shutdown(asio::ip::tcp::socket::shutdown_both);
  // A passive socket listens for incomming connection requests
  m_acceptor.async_accept(*sock,
	[this, sock](
		const boost::system::error_code& ec){
		  try{
			if(ec) 
				throw ec;
			
			onAccept(ec, sock);
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
void Acceptor::onAccept(const boost::system::error_code& ec,
	std::shared_ptr<asio::ip::tcp::socket> sock) {
	if (!ec) {
		(new Service(sock))->start_handling();
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