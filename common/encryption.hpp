#ifndef PASSWORD_ENCRYPTION_H
#define PASSWORD_ENCRYPTION_H

#include <iostream>
#include "cstring"



using namespace std;
string hashPassword(string pswd,string key){
	/*Method generating/decoding a hash*/
 	string output = "";
 	
 	for (unsigned int temp = 0; temp < pswd.size(); temp++){
  		output += pswd[temp] ^ (int(key[temp%key.size()]) + temp) % 255;
 	}
 	return output;
}

string encryptS(string pswd,string key){
	/*Method that encrypts pswd against a given key
	*returns a string consisting in the hashed password
	*/
	return (hashPassword(pswd,key));
}


string decryptS(string pswd,string key){
	/*Method that decrypts pswd against a given key
	*returns a string representing the password
	*/
	return (hashPassword(pswd,key));
}

#endif // ENCRYPTION_H


