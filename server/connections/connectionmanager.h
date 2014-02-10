#ifndef DEFINE_CONNECTIONMANAGER_HEADER
#define DEFINE_CONNECTIONMANAGER_HEADER

#include <iostream>
#include <map>
#include <stdexcept>
#include <netinet/in.h>

class ConnectionError : public std::runtime_error {
    public: using std::runtime_error::runtime_error;
}; 

class User {
    public: User(){std::cout << "Nouvel utilisateur" << std::endl;}
};

class ConnectionManager {
    private:
        struct sockaddr_in _bind_addr;
        int _sockfd;
        std::map<int,User> _clients;
        void _addClient(void);
        bool _readFromFD(int fd);
        void _removeClient(int client_id);
    public:
        ConnectionManager( 
            const char *bind_addr="127.0.0.1", 
            unsigned short bind_port=32123,
            int max_clients=25
        );
        ~ConnectionManager();
        void mainloop(void);
};

#endif
