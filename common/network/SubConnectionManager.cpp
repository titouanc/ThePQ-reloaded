#include "SubConnectionManager.hpp"
#include <Constants.hpp>

using namespace net;

/* ===== SubConnectionManager ===== */
SubConnectionManager::SubConnectionManager(
    SharedQueue<Message> & incoming_queue,
    SharedQueue<Message> & outgoing_queue,
    BaseConnectionManager & parent
) : 
BaseConnectionManager(incoming_queue, outgoing_queue), _parent(parent)
{}

bool SubConnectionManager::acquireClient(int client_id)
{
    if (_parent.removeClient(client_id)){
        addClient(client_id);
        return true;
    }
    return false;
}

bool SubConnectionManager::releaseClient(int client_id)
{
    if (removeClient(client_id)){
        _parent.addClient(client_id);
        return true;
    }
    return false;
}

void SubConnectionManager::_doDisconnect(int fd)
{
    /* notify parent that we lost a client */
    BaseConnectionManager::_doDisconnect(fd);
    JSON::Dict payload = {
        {"type", JSON::String(MSG::DISCONNECT)},
        {"client_id", JSON::Integer(fd)}
    };
    _parent.transmit(Message(fd, payload.clone()));
}

SubConnectionManager::~SubConnectionManager()
{
    /* Give back fd to parent connection on destruction */
    for (iterator client=_iterClients(); client!=_iterEnd();){
        iterator next = client;
        next++;
        releaseClient(*client);
        client = next;
    }
}
