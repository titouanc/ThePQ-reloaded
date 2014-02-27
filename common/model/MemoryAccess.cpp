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
#include <model/Team.hpp>
#include <model/Gear.hpp>
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
std::string MemoryAccess::getTeamInfosPath(std::string username){
	return getUserDirectory(username) + memory::TEAM_INFOS_FILE + memory::FILE_FORMAT;
}
std::string MemoryAccess::getSkelPath(std::string file){
	return memory::SKEL_DIR + file + memory::FILE_FORMAT;
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
	std::string path = getSalePath(sale.getID());
	JSON::Dict toSave = sale;
	toSave.save(path.c_str());
}
void MemoryAccess::save(Team& team){
	std::string path = getTeamInfosPath(team.getOwner());
	JSON::Dict toSave = team;
	toSave.save(path.c_str());
}

void MemoryAccess::load(Player& player){
	JSON::Value *loaded = JSON::load(getPlayerPath(player.getOwner(),player.getMemberID()).c_str());
	player = DICT(loaded);
	delete loaded;
}

void MemoryAccess::load(User& user){
	JSON::Value *loaded = JSON::load(getUserPath(user.getUsername()).c_str());
	user = &DICT(loaded); //Constructor by pointer in User ... 
	delete loaded;
}

void MemoryAccess::load(Sale& sale){
	JSON::Value *loaded = JSON::load(getSalePath(sale.getID()).c_str());
	sale = DICT(loaded);
	delete loaded;
}
void MemoryAccess::load(Installation& install){
	JSON::Value *loaded = JSON::load(getInstallationPath(install.getOwner(),install.getName()).c_str());
	install = DICT(loaded);
	delete loaded;
}
void MemoryAccess::load(Team& team){
	JSON::Value *loaded = JSON::load(getTeamInfosPath(team.getOwner()).c_str());
	team = DICT(loaded);
	delete loaded;
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

void MemoryAccess::load(std::vector<Installation> &toFill,std::string username){
	JSON::List installs = loadFilesInVec(getInstallationsDirectory(username));
	for(size_t i=0;i<installs.len();++i){
		toFill.push_back(DICT(installs[i]));
	}
}

void MemoryAccess::load(std::vector<Player> &toFill,std::string username){
	JSON::List players = loadFilesInVec(getPlayersDirectory(username));
	for(size_t i=0;i<players.len();++i){
		toFill.push_back(DICT(players[i]));
	}
}

void MemoryAccess::load(std::vector<Sale*> &toFill){
	JSON::List sales = loadFilesInVec(memory::MARKET_PATH);
	for(size_t i=0;i<sales.len();++i){
		toFill.push_back(new Sale(DICT(sales[i])));
	}
}

void MemoryAccess::loadSkel(Broomstick& broom){
	JSON::Value *loaded = JSON::load(getSkelPath(memory::BROOM_FILE).c_str());
	broom = DICT(loaded);
	delete loaded;
}

void MemoryAccess::loadSkel(Jersey& jersey){
	JSON::Value *loaded = JSON::load(getSkelPath(memory::JERSEY_FILE).c_str());
	jersey = DICT(loaded);
	delete loaded;
}

void MemoryAccess::loadSkel(std::vector<Installation> &vec){
	JSON::Value *loaded = JSON::load(getSkelPath(memory::INSTS_SKEL_FILE).c_str());
	JSON::List & insts = LIST(loaded);
	for(size_t i = 0;i<insts.len();++i){
		vec.push_back(DICT(insts[i]));
	}
	delete loaded;
}


void MemoryAccess::removeObject(Player &player){
	remove(getPlayerPath(player.getOwner(), player.getMemberID()).c_str());
}

void MemoryAccess::removeObject(Sale &sale){
	remove(getSalePath(sale.getID()).c_str());
}

void MemoryAccess::removeObject(User &user){
	remove(getUserPath(user.getUsername()).c_str());
}

void MemoryAccess::removeObject(Installation &install){
	remove(getInstallationPath(install.getOwner(),install.getName()).c_str());
}
