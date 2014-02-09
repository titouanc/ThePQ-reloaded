#ifndef __MESSAGE_HPP
#define __MESSAGE_HPP

#include "json.h"

namespace net
{
	class Message : public JSON::Dict
	{
	public:		
		enum Method
		{
			REQUEST,
			RESPONSE
		};
		
		enum Request
		{
			LOGIN,
			SUBSCRIBE
		};
		
		Message(Request request, Method method, JSON::Value* data) : JSON::Dict()
		{
			set("Request", JSON::Integer(request));
			set("Method", JSON::Integer(method));
			set("Data", *data);
		}
		
		Message(JSON::Dict const & dict)
		{
			set("Request", *(dict.get("Request")));
			set("Method", *(dict.get("Method")));
			set("Data", *(dict.get("Data")));
		}
		
		~Message()
		{
			
		}	
	};
}

#endif // __MESSAGE_HPP
