
#include "Acceptor.h"
#include "Service.h"

Acceptor::Acceptor(asio::io_context& ios, unsigned short port_num) :
  m_ios(ios),
  m_acceptor(m_ios,
  tcp::endpoint(
  asio::ip::address_v4::any(),
  port_num)),
  m_isStopped(false)
{}

	// Start accepting incoming connection requests.
void Acceptor::Start() {
  m_acceptor.listen();
  InitAccept();
}

	// Stop accepting incoming connection requests.
void Acceptor::Stop() {
  m_isStopped.store(true);
}

void Acceptor::InitAccept() {
  std::shared_ptr<tcp::socket>
	sock(new tcp::socket(m_ios));
  std::cout<<"init accept\n ";

  m_acceptor.async_accept(*sock.get(),
	[this, sock](
		const boost::system::error_code& error){
          std::cout<<"handle init accept\n ";
		  try{
			onAccept(error, sock);
		  } 
		  catch(boost::system::error_code& ec) {
			std::cout << "Error occured! Error code = "
			<< ec.value()
			<< ". Message: " << ec.message();
		  }
		  
		});
}

void Acceptor::onAccept(const boost::system::error_code& ec,
  std::shared_ptr<tcp::socket> sock) {
    std::cout<<"ec value: "<<ec.value()<<std::endl;
	if (ec.value() == 0) {
	  (new Service(sock))->start();
      //std::make_shared<Service>((*sock.get())))->start();
      std::cout<<"make pointer shared\n";
	}
	else {
	  throw ec;
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