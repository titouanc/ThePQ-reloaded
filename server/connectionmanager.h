#ifndef DEFINE_CONNECTIONMANAGER_HEADER
#define DEFINE_CONNECTIONMANAGER_HEADER

#include <iostream>
#include <map>
#include <stdexcept>
#include <netinet/in.h>
#include "sharedqueue.h"
#include <json.h>

class ConnectionError : public std::runtime_error {
    public: using std::runtime_error::runtime_error;
}; 

class User {
    public: 
        User(){std::cout << "Nouvel objet utilisateur" << std::endl;}
        User(User const & other){std::cout << "Copie objet utilisateur" << std::endl;}
        User(User const && other){std::cout << "Transfert objet utilisateur" << std::endl;}
        ~User(){std::cout << "Destruction objet utilisateur" << std::endl;}
};

struct Message {
    int peer_id;
    JSON::Value *data;
    Message(int p, JSON::Value *d) : peer_id(p), data(d) {}
};

class ConnectionManager {
    private:
        struct sockaddr_in _bind_addr;
        int _sockfd;
        std::map<int,User> _clients;
        void _addClient(void);
        JSON::Value *_readFrom(int fd);
        bool _writeTo(int fd, JSON::Value *obj);
        void _removeClient(int client_id);
    public:
        ConnectionManager( 
            const char *bind_addr="127.0.0.1", 
            unsigned short bind_port=32123,
            int max_clients=25
        );
        ~ConnectionManager();
        void mainloop(SharedQueue<Message> & incoming);

        /* Getters */
        const char *ip(void) const;
        unsigned short port(void) const;
};

#endif
