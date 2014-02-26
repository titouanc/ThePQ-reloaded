#include "MemoryAccess.hpp"
#include <Exception.hpp>
#include "dirent.h"
#include <stdio.h>
#include <Constants.hpp>
#include <sys/stat.h>
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

void MemoryAccess::save(std::string datatype, JSON::Dict &tosave){
	std::string owner = "";
	std::string type = "";
	int id = -1;
	if(tosave.hasKey(net::MSG::USERNAME)) {owner=STR(tosave.get(net::MSG::USERNAME)).value();}
	if(tosave.hasKey(net::MSG::PLAYER_ID)){id=INT(tosave.get(net::MSG::PLAYER_ID));}
	if(tosave.hasKey(memory::INST_TYPE))  {type=STR(tosave.get(memory::INST_TYPE)).value();}
	if(datatype == memory::PLAYER){
		if(id == -1 || owner.empty())
			throw IncompleteDictException();
		tosave.save(getPlayerPath(owner, id).c_str());
	}
	else if(datatype == memory::USER){
		if(owner.empty())
			throw IncompleteDictException();
		mkdir(getUserDirectory(owner).c_str(), 0755);
		mkdir((getUserDirectory(owner)+memory::PLAYERS_DIR).c_str(), 0755);
		mkdir((getUserDirectory(owner)+memory::INSTALLATIONS_DIR).c_str(), 0755);
		tosave.save(getUserPath(owner).c_str());
	}
	else if(datatype == memory::INSTALLATION){
		if(owner.empty() || type.empty())
			throw IncompleteDictException();
		tosave.save(getInstallationPath(owner, type));
	}
	else if(datatype == memory::SALE){
		if(id == -1)
			throw IncompleteDictException();
		tosave.save(getSalePath(id));
	}
}

JSON::Dict MemoryAccess::load(std::string datatype, std::string owner, int id){
	if(datatype != memory::PLAYER)
		throw wrongLoadFunctionException();
	JSON::Value *loaded = JSON::load(getPlayerPath(owner,id).c_str());
	JSON::Dict ret = DICT(loaded);
	delete loaded;
	return ret;
}

JSON::Dict MemoryAccess::load(std::string datatype, std::string username){
	if(datatype != memory::USER)
		throw wrongLoadFunctionException();
	JSON::Value *loaded = JSON::load(getUserPath(username).c_str());
	JSON::Dict ret = DICT(loaded);
	delete loaded;
	return ret;
}

JSON::Dict MemoryAccess::load(std::string datatype, std::string owner, std::string type){
	if(datatype != memory::INSTALLATION)
		throw wrongLoadFunctionException();
	JSON::Value *loaded = JSON::load(getInstallationPath(owner,type));
	JSON::Dict ret = DICT(loaded);
	delete loaded;
	return ret;
}

/*Only for sales*/
JSON::Dict MemoryAccess::load(std::string datatype, int id){
	if(datatype != memory::SALE)
		throw wrongLoadFunctionException();
	JSON::Value *loaded = JSON::load(getSalePath(id));
	JSON::Dict ret = DICT(loaded);
	delete loaded;
	return ret;
}

JSON::List MemoryAccess::loadFilesInDir(std::string directory){
	JSON::List ret;
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
					JSON::Dict cpy = DICT(loaded);
					ret.append(cpy);
					delete loaded;
				}
			}
		}
	}
	closedir(dir);
	return ret;
}

JSON::List MemoryAccess::loadList(std::string datatype){
	if(datatype != memory::SALES_LIST)
		throw wrongLoadFunctionException();
	JSON::List cont;
	std::string path = memory::MARKET_PATH;
	return loadFilesInDir(path);
}

JSON::List MemoryAccess::loadList(std::string datatype, std::string username){
	std::string path;
	if(datatype == memory::INST_LIST)
		path = getInstallationsDirectory(username);
	else if(datatype == memory::PLAYERS_LIST)
		path = getPlayersDirectory(username);
	else
		throw wrongLoadFunctionException();
	return loadFilesInDir(path);
}

void MemoryAccess::removeFile(std::string datatype, std::string username){
	if(datatype != memory::USER)
		throw wrongRemoveFunctionException();
	remove(getUserPath(username).c_str());
}

void MemoryAccess::removeFile(std::string datatype, int id){
	if(datatype != memory::SALE)
		throw wrongRemoveFunctionException();
	remove(getSalePath(id).c_str());
}

void MemoryAccess::removeFile(std::string datatype, std::string owner, int id){
	if(datatype != memory::PLAYER)
		throw wrongRemoveFunctionException();
	remove(getPlayerPath(owner, id).c_str());
}

void MemoryAccess::removeFile(std::string datatype, std::string owner, std::string type){
	if(datatype != memory::INSTALLATION)
		throw wrongRemoveFunctionException();
	remove(getInstallationPath(owner,type).c_str());
}