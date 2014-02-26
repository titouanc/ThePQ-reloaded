#include "MemoryAccess.hpp"
#include <Exception.hpp>
#include "dirent.h"
#include <stdio.h>
#include <Constants.hpp>
#include <sys/stat.h>
#include <model/Player.hpp>
#include <model/Sale.hpp>
#include <model/Installation.hpp>
#include <model/User.hpp>
#include <iostream>

std::string MemoryAccess::getUserDirectory(std::string username){
	return memory::USERS_DIR + username + "/";
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

void MemoryAccess::save(Installation& install){
	std::string path = getInstallationPath(install.getOwner(), install.getName());
	JSON::Dict toSave = install;
	toSave.save(path.c_str());
}
void MemoryAccess::save(User& user){
	std::string username = user.getUsername();
	mkdir(getUserDirectory(username).c_str(), 0755);
    mkdir((getUserDirectory(username)+memory::PLAYERS_DIR).c_str(), 0755);
    mkdir((getUserDirectory(username)+memory::INSTALLATIONS_DIR).c_str(), 0755);
	std::string path = getUserPath(user.getUsername());
	JSON::Dict toSave = user;
	toSave.save(path.c_str());
}
void MemoryAccess::save(Player& player){
	std::string path = getPlayerPath(player.getOwner(), player.getMemberID());
	JSON::Dict toSave = player;
	toSave.save(path.c_str());
}
void MemoryAccess::save(Sale& sale){
	mkdir(memory::MARKET_PATH.c_str(), 0755);
	std::string path = getSalePath(sale.getID());
	JSON::Dict toSave = sale;
	toSave.save(path.c_str());
}

Player MemoryAccess::load(Player& player){
	JSON::Value *loaded = JSON::load(getPlayerPath(player.getOwner(),player.getMemberID()).c_str());
	Player ret(DICT(loaded));
	delete loaded;
	return ret;
}

User MemoryAccess::load(User& user){
	JSON::Value *loaded = JSON::load(getUserPath(user.getUsername()).c_str());
	User ret(&DICT(loaded)); //Constructor by pointer in User ... 
	delete loaded;
	return ret;
}

Sale MemoryAccess::load(Sale& sale){
	JSON::Value *loaded = JSON::load(getSalePath(sale.getID()).c_str());
	Sale ret(DICT(loaded));
	delete loaded;
	return ret;
}
Installation MemoryAccess::load(Installation& install){
	JSON::Value *loaded = JSON::load(getInstallationPath(install.getOwner(),install.getName()).c_str());
	Installation ret(DICT(loaded));
	delete loaded;
	return ret;
}

JSON::List MemoryAccess::loadFilesInVec(std::string directory){/*Check for memleaks*/
	JSON::List ret = JSON::List();
	DIR* dir = NULL;
	struct dirent* file = NULL;
	dir = opendir(directory.c_str());
	if(dir!=NULL){		
		while((file = readdir(dir)) != NULL){
			std::string str1 = file->d_name;
			std::string str2 = memory::FILE_FORMAT;
			if(str1.size()>str2.size()){
				if(str1.compare(str1.size()-str2.size(),str2.size(),str2) == 0){
					JSON::Value* loaded = JSON::load((directory+file->d_name).c_str());
					ret.append(DICT(loaded));
					delete loaded;
				}
			}
		}
	}
	closedir(dir);
	return ret;
}

void MemoryAccess::load(std::vector<Installation> *toFill,std::string username){
	JSON::List installs = loadFilesInVec(getInstallationsDirectory(username));
	for(size_t i=0;i<installs.len();++i){
		toFill->push_back(DICT(installs[i]));
	}
}

void MemoryAccess::load(std::vector<Player> *toFill,std::string username){
	JSON::List players = loadFilesInVec(getPlayersDirectory(username));
	for(size_t i=0;i<players.len();++i){
		toFill->push_back(DICT(players[i]));
	}
}

void MemoryAccess::removeFile(Player &player){
	remove(getPlayerPath(player.getOwner(), player.getMemberID()).c_str());
}

void MemoryAccess::removeFile(Sale &sale){
	remove(getSalePath(sale.getID()).c_str());
}

void MemoryAccess::removeFile(User &user){
	remove(getUserPath(user.getUsername()).c_str());
}

void MemoryAccess::removeFile(Installation &install){
	remove(getInstallationPath(install.getOwner(),install.getName()).c_str());
}
