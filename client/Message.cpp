#include "Message.hpp"
#include <string>

std::string Message::splashScreen(){
string message;
	message+="	             _____ _            ____            \n";
	message+="	            |_   _| |__   ___  |  _ \\ _ __ ___ \n";
	message+="		      | | | '_ \\ / _ \\ | |_) | '__/ _ \\\n ";
	message+="		      | | | | | |  __/ |  __/| | | (_) |\n";
	message+="		      |_| |_| |_|\\___| |_|   |_|  \\___/ \n";
	message+="		   ___        _     _     _ _ _       _ \n";
	message+="		  / _ \\ _   _(_) __| | __| (_) |_ ___| |__ \n";
	message+="		 | | | | | | | |/ _` |/ _` | | __/ __| '_ \\ \n";
	message+="		 | |_| | |_| | | (_| | (_| | | |_ (__| | | |\n";
	message+="		  \\__\\_ \\__,_|_|\\__,_|\\__,_|_|\\__\\___|_| |_|\n";
	message+= "        =======||\\    	Just a game, no bullshit!\n";
	message+= "    ===========|| \\___________________________  ___            |\n";
	message+= "  =============|| |___________________________==___|>        - * -\n";
	message+= "    ===========|| /                                            |\n";
	message+= "        =======||/ 		\n";
	message+="		  _ __ ___   __ _ _ __   __ _  __ _  ___ _ __ \n";
	message+="		 | '_ ` _ \\ / _` | '_ \\ / _` |/ _` |/ _ \\ '__|\n";
	message+="		 | | | | | | (_| | | | | (_| | (_| |  __/ |\n";
	message+="		 |_| |_| |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_|\n";
	message+="   		               	              |___/\n";
	message+="			Welcome to The Pro Quidditch Manager 2014!\n";
return message;
}

Message::goodBye(){
	string message;
	message+="Bye bye baby";
	return message;
}