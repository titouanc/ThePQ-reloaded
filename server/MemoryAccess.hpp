#ifndef __MEMORYACCESS_HPP
#define __MEMORYACCESS_HPP

#include <json/json.hpp>
#include <string>
#include <vector>

class Player;
class Sale;
class User; 
class Installation;
namespace MemoryAccess{
	std::string getUserDirectory(std::string);
	std::string getUserPath(std::string);
	std::string getPlayersDirectory(std::string);
   	std::string getPlayerPath(std::string, int);
   	std::string getInstallationsDirectory(std::string);
   	std::string getInstallationPath(std::string, std::string);
   	std::string getSalePath(int);

   	std::string getSavePath(Player&);
  	std::string getSavePath(Sale&);
   	std::string getSavePath(User&);
   	std::string getSavePath(Installation&);

	/* Saves the object given as parameter. An operator JSON::Dict() for this object is required. */
   	template<typename T> void save(T&);

   	/* Saves the user's infos and creates the needed directories. */
   	template<> void save(User&);
   	
   	/* Saves a vector of objects depending on each owner's (or/and ID). */
   	template<typename T> void save(std::vector<T>&);
   	template<typename T> void save(std::vector<T*>&);

   	/* Loads the object in memory corresponding to the parameter, the given parameter therefore
   	needs some attributes :
   		+ Sale : needs ID;
   		+ Player : needs owner (username) and ID;
   		+ User : needs username;
   		+ Installation : needs owner (username) and type (Fan Shop, etc.). */
   	template<typename T> T load(T&);

   	/* The next functions will fill the given vector with respectively :
   		+ all the sales in memory;
   		+ all the players of the user given as parameter;
   		+ all the installations of the user given as parameter.
   	N.B. : the values in the vectors are dynamically allocated ! */
   	void load(std::vector<Sale*>& sales);
   	void load(std::vector<Player*>& players, std::string username);
   	void load(std::vector<Installation*>& installs, std::string username);
   	template<typename T> void loadFilesInVector(std::vector<T*>& vec, std::string directory);

   	template<typename T> void removeFile(T&);
}
#endif