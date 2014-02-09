#ifndef __MESSAGE_HPP
#define __MESSAGE_HPP

namespace net
{
	class Message : public JSON::Dict
	{
	public:
		Message(Request request, Method method, JSON::Value* data) : JSON::Dict()
		{
			set("Request", JSON::Integer(request));
			set("Method", JSON::Integer(method));
			set("Data", *data);
		}
		~Message();
		
		enum Method
		{
			REQUEST,
			RESPONSE
		}
		
		enum Request
		{
			LOGIN,
			SUBSCRIBE
		}		
	};
}

#endif // __MESSAGE_HPP
