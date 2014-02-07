#include "Listener.hpp"

Socket::Status Listener::listen(int portNo)
{
    close();
    if (!create())
	return Status::ERROR;
    /* BINDING */
    _servAddr.sin_family = AF_INET;
    _servAddr.sin_addr.s_addr = INADDR_ANY;
    _servAddr.sin_port = htons(portNo);
    if (bind(_sockfd, (struct sockaddr *) &_servAddr, sizeof(_servAddr)) <
	0)
	return Status::ERROR;

    /* LISTENING */
    if (::listen(_sockfd, CONNECTION_NB) < 0)
	return Status::ERROR;

    return Status::OK;
}

Socket::Status Listener::accept(TcpSocket & sock)
{
    if (!isOpen())
	return Status::ERROR;

    socklen_t clilen = sizeof(_cliAddr);
    sock._sockfd =::accept(_sockfd, (struct sockaddr *) &_cliAddr,
			   &clilen);
    if (sock._sockfd < 0)
	return Status::ERROR;
    sock._servAddr = this->_servAddr;
    sock._cliAddr = this->_cliAddr;

    return Status::OK;
}
