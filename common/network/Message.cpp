#include "net.hpp"
		
net::Message::Message(net::Message::Request request, net::Message::Method method, JSON::Value *data) : JSON::Dict()
{
	set("Request", JSON::Integer(request));
	set("Method", JSON::Integer(method));
	set("Data", *data);
}
