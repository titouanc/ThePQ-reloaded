#ifndef DEFINE_CONNECTIONMANAGER_HEADER
#define DEFINE_CONNECTIONMANAGER_HEADER

#include <iostream>
#include <set>
#include <stdexcept>
#include <netinet/in.h>
#include <pthread.h>
#include "../sharedqueue.h"
#include <json.h>

class ConnectionError : public std::runtime_error {
    public: using std::runtime_error::runtime_error;
}; 

struct Message {
    int peer_id;
    JSON::Value *data;
    Message(int p, JSON::Value *d) : peer_id(p), data(d) {}
};

class ConnectionManager {
    private:
        /* Bound IP structure */
        struct sockaddr_in _bind_addr;
        /* Listener socket */
        int _sockfd;
        /* Currently connected clients */
        std::set<int> _clients;
        /* Communication thread */
        pthread_t   _io_thread;
        /* isRunning() mutex */
        pthread_mutex_t _mutex;
        /* Is IO thread running ? */
        bool _running;
        /* Incoming message queue */
        SharedQueue<Message> & _incoming;


        /* Accept a new connection; return 
           {"__type__": "CONNECT", "client_id": <int>} */
        JSON::Value *_addClient(void);
        /* Remove an existing connection; return 
           {"__type__": "DISCONNECT", "client_id": <int>} */
        JSON::Value *_removeClient(std::set<int>::iterator position);
        /* Read and parse a message from given fd */
        JSON::Value *_readFrom(int fd);
        bool _writeTo(int fd, JSON::Value *obj);
    public:
        explicit ConnectionManager(
            SharedQueue<Message> & incoming_queue,
            const char *bind_addr="127.0.0.1", 
            unsigned short bind_port=32123,
            int max_clients=25
        );
        ~ConnectionManager();

        /* Main IO loop: accept connections; 
         *               read messages from clients; 
         *               feed an incoming queue 
         */
        void mainloop(void);

        /* Start the main IO loop in a background thread */
        bool start(void);

        /* Stop the main IO loop */
        bool stop(void);

        /* Getters */
        const char *ip(void) const;
        unsigned short port(void) const;
        bool isRunning(void);
};

#endif
