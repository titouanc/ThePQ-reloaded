#include <Constants.hpp>
#include <MemoryAccess.hpp>
#include "dirent.h"

std::string MemoryAccess::getUserDirectory(std::string username){
	return memory::USERS_PATH + username + "/";
}
std::string MemoryAccess::getUserPath(std::string username){
    return getUserDirectory(username) + memory::USER_FILE + memory::FILE_FORMAT;
}
std::string MemoryAccess::getPlayersDirectory(std::string username){
	return getUserDirectory(username) + memory::PLAYERS_DIR;
}
std::string MemoryAccess::getPlayerPath(std::string username, int id){
	return getPlayersDirectory(username) + std::to_string(id) + memory::FILE_FORMAT;
}
std::string MemoryAccess::getInstallationsDirectory(std::string username){
	return getUserDirectory(username) + memory::INSTALLATIONS_DIR;
}
std::string MemoryAccess::getInstallationPath(std::string username, std::string type){
	return getInstallationsDirectory(username) + type + memory::FILE_FORMAT;
}
std::string MemoryAccess::getSalePath(int id){
	return memory::MARKET_PATH + std::to_string(id) + memory::SALE_FILE + memory::FILE_FORMAT;
}

std::string MemoryAccess::getSavePath(Player& player){
	return getPlayerPath(player.getOwner(), player.getMemberID());
}
std::string MemoryAccess::getSavePath(Sale& sale){
	return getSalePath(sale.getID());
}
std::string MemoryAccess::getSavePath(User& user){
	return getUserPath(user.getUsername());
}
std::string MemoryAccess::getSavePath(Installation& install){
	return getInstallationPath(install.getOwner(), install.getName());
}

template<typename T> void MemoryAccess::save(T& t){
	JSON::Dict toSave = t;
	toSave.save(getSavePath(t).c_str());
}

template<> void MemoryAccess::save(User& user){
	mkdir(getUserPath(user.getUsername()).c_str(), 0755);
	mkdir((getUserPath(user.getUsername())+memory::PLAYERS_DIR).c_str(), 0755);
	mkdir((getUserPath(user.getUsername())+memory::INSTALLATIONS_DIR).c_str(), 0755);
	JSON::Dict userInfos = user;
	user.save(getSavePath(user).c_str());
}

template<typename T> void MemoryAccess::save(std::vector<T>& vec){
	for(size_t i=0; i<vec.size();++i){
		save(vec[i]);
	}
}

template<typename T> T MemoryAccess::load(T& t){
	JSON::Value* loaded = JSON::load(getSavePath(t).c_str());
	T ret(DICT(loaded));
	delete loaded;
	return ret;
}

void MemoryAccess::load(std::vector<Sale*>& sales){
	loadFilesInVector(sales, memory::MARKET_PATH);
}

void MemoryAccess::load(std::vector<Player*>& players, std::string username){
	loadFilesInVector(players, getPlayersDirectory(username));
}

void MemoryAccess::load(std::vector<Installation*>& installs, std::string username){
	loadFilesInVector(installs, getInstallationsDirectory(username));
}

template<typename T> void MemoryAccess::loadFilesInVector(std::vector<T*>& vec, std::string directory){
	DIR* rep = NULL;
	struct dirent* file = NULL;
	dir = opendir(directory.c_str());
	if(dir==NULL)
		throw JSON::IOError();
	while((file = readdir(dir)) != NULL){
		std::string filename = file->d_name;
		if(filename[0] != "."){
			JSON::Value* loaded = JSON::load((directory+filename).c_str());
			sales.push_back(new T(DICT(loaded)));
			delete loaded;
		}
	}
	closedir(dir);
}
