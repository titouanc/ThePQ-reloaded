#ifndef __NET_EXCEPTION_HPP
#define __NET_EXCEPTION_HPP

namespace net
{
	class ConnectionFailedException : public std::runtime_error {
	public:
		using std::runtime_error::runtime_error;
		ConnectionFailedException():runtime_error(std::string("connect() failed: ")+strerror(errno))
		{}
	};

	class DisconnectedException : public std::runtime_error {
	public:
		using std::runtime_error::runtime_error;
		DisconnectedException():runtime_error(std::string("host disconnected")+strerror(errno))
		{}
	};	
}
	
#endif // __NET_EXCEPTION_HPP
