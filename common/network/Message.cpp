#include "net.hpp"
		
net::Message::Message(net::Message::Request request, net::Message::Method method, JSON::Value *data) : JSON::Dict()
{
	set("Request", request);
	set("Method", method);
	set("Data", *data);
}
