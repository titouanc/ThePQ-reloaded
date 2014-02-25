#ifndef __MEMORYACCESS_HPP
#define __MEMORYACCESS_HPP

#include <json/json.hpp>
#include <Constants.hpp>
#include <string>
#include <vector>

namespace MemoryAccess{
	std::string getUserPath(std::string username){
    	return memory::USERS_PATH + username + "/";
    }
    std::string getUserFile(std::string username){
    	return getUserPath(username) + memory::USER_FILE + memory::FILE_FORMAT;
    }
   	std::string getPlayerFile(std::string username, int id){
   		return getUserPath(username) + memory::PLAYERS_DIR + std::to_string(id) + memory::FILE_FORMAT;
   	}
   	std::string getInstallationFile(std::string username, std::string type){
   		return getUserPath(username) + memory::INSTALLATIONS_DIR + type + memory::FILE_FORMAT;
   	}
   	std::string getSalePath(int id){
   		return memory::MARKET_PATH + std::to_string(id) + memory::SALE_FILE + memory::FILE_FORMAT;
   	}

   	std::string getSavePath(Player& player){
   		return getPlayerFile(player.getOwner(), player.getMemberID());
   	}
  	std::string getSavePath(Sale& sale){
   		return getSalePath(sale.getID());
   	}
   	std::string getSavePath(User& user){
   		return getUserPath(user.getUsername());
   	}

   	template<typename T> void save(T& t){
   	/* Saves the object given as parameter. An operator JSON::Dict() for this object is needed. */
   		JSON::Dict toSave = t;
   		toSave.save(getSavePath(t).c_str());
   	}
   	template<> void save(User& user){
   	/* Global saves for an user (user infos + installations + players). */
   		mkdir(getUserPath(user.getUsername()).c_str(), 0755);
   		mkdir((getUserPath(user.getUsername())+PLAYERS_DIR).c_str(), 0755);
   		mkdir((getUserPath(user.getUsername())+INSTALLATIONS_DIR).c_str(), 0755);
   		save(user.getPlayers());
   		save(user.getInstallations());
   		JSON::Dict userInfos = user;
   		user.save(getUserFile(user.getUsername()).c_str());
   	}

   	template<> void save(std::vector<Player>& players){
   	/* Saves a vector of players depending on each owner's. */
   		for(size_t i=0; i<players.size();++i){
   			JSON::Dict toSave = player[i];
   			toSave.save(getPlayerFile(player[i].getOwner(),player[i].getMemberID()));
   		}
   	}

	template<> void save(std::vector<Installation>& installs){

   	}

}
#endif