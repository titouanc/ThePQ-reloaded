

#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <iostream>
#include "cstring"



using namespace std;
string hash(string pswd,string key){
	/*Method generating/decoding a hash*/
 	string output = "";
 	
 	for (unsigned int temp = 0; temp < pswd.size(); temp++){
  		output += pswd[temp] ^ (int(key[temp%key.size()]) + temp) % 255;
 	}
 	return output;
}

string encrypt(string pswd,string key){
	/*Method that encrypts pswd against a given key
	*returns a string consisting in the hashed password
	*/
	return (hash(pswd,key));
}


string decrypt(string pswd,string key){
	/*Method that decrypts pswd against a given key
	*returns a string representing the password
	*/
	return (hash(pswd,key));
}

#endif // ENCRYPTION_H


