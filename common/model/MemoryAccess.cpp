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
#include <model/Championship.hpp>
#include <iostream>

/**
 * Method retrieving the users directory
 * @param std::string : name of the user
 * @return std::string : path to the users directory
 */
std::string MemoryAccess::getUserDirectory(std::string username){
	return memory::USERS_DIR + username + "/";
}
/**
 * Method retrieving the full users directory path
 * @param std::string : name of the user
 * @return std::string : full path to the users directory
 */
std::string MemoryAccess::getUserPath(std::string username){
    return getUserDirectory(username) + memory::USER_FILE + memory::FILE_FORMAT;
}
/**
 * Method retrieving the player directory
 * @param std::string : name of the user
 * @return std::string : path to the  directory
 */
std::string MemoryAccess::getPlayersDirectory(std::string username){
	return getUserDirectory(username) + memory::PLAYERS_DIR;
}

/**
 * Method retrieving the player directory full path
 * @param std::string : name of the user
 * @return std::string : full path to the  players directory
 */
std::string MemoryAccess::getPlayerPath(std::string username, int id){
	return getPlayersDirectory(username) + std::to_string(id) + memory::FILE_FORMAT;
}

/**
 * Method retrieving path to installations directory
 * @param std::string : username of user whose Installations path should be returned
 * @return std::string : path to the users intallations
 */
std::string MemoryAccess::getInstallationsDirectory(std::string username){
	return getUserDirectory(username) + memory::INSTALLATIONS_DIR;
}
std::string MemoryAccess::getInstallationPath(std::string username, std::string type){
	return getInstallationsDirectory(username) + type + memory::FILE_FORMAT;
}

/**
 * Method retrieving the path to the folder containing the sales
 * @param int : id of the sale whose path will be retrieved
 * @return std::string : path of the file containing the sale (with the provided id)
 */
std::string MemoryAccess::getSalePath(int id){
	return memory::MARKET_PATH + std::to_string(id) + memory::SALE_FILE + memory::FILE_FORMAT;
}

/**
 * Method retrieving the path to the folder containing the team attributes
 * @param std::string : name of the teams owner
 * @return std::string : full path for the team attributes folder
 */
std::string MemoryAccess::getTeamInfosPath(std::string username){
	return getUserDirectory(username) + memory::TEAM_INFOS_FILE + memory::FILE_FORMAT;
}
std::string MemoryAccess::getSkelPath(std::string file){
	return memory::SKEL_DIR + file + memory::FILE_FORMAT;
}

/**
 * Method retrieving the path to the teamnames
 * @return std::string : path to the file containing names of the teams
 */
std::string MemoryAccess::getTeamNamesPath(){
	return memory::GLOBAL_DATA_DIR + memory::TEAMNAMES_FILE + memory::FILE_FORMAT;
}

/**
 * Method retrieving the path of the admin user
 * @param std::string : name of the admin
 * @return std::string : full path to the admin
 */
std::string MemoryAccess::getAdminPath(std::string adminname){
	return memory::ADMIN_DIR + adminname + "/" + memory::ADMIN_FILE + memory::FILE_FORMAT;
}

/**
 * Method retrieving the path of all the usernames
 * @return std::string : full path to the usernames
 */
std::string MemoryAccess::getUserNamesPath(){
	return memory::GLOBAL_DATA_DIR + memory::USERNAMES_FILE + memory::FILE_FORMAT;
}

/**
 * Method retrieving the path of championship data
 * @param std::string : name of the championship
 * @return std::string : path of the championship data
 */
std::string MemoryAccess::getChampionshipPath(std::string name){
	return memory::CHAMPIONSHIPS_DIR + name + memory::FILE_FORMAT;
}

/**
 * Method saving the installations of a user
 * @param Installation * : pointer to a users installations
 */
void MemoryAccess::save(Installation* install){
	std::string path = getInstallationPath(install->getOwner(), install->getName());
	JSON::Dict toSave = *install;
	toSave.save(path.c_str());
}

/**
 * Method saving all user adata
 * @param User : users whose data shall be saved
 */
void MemoryAccess::save(User& user){
	std::string username = user.getUsername();
	mkdir(memory::USERS_DIR.c_str(), 0755);
	mkdir(getUserDirectory(username).c_str(), 0755);
    mkdir((getUserDirectory(username)+memory::PLAYERS_DIR).c_str(), 0755);
    mkdir((getUserDirectory(username)+memory::INSTALLATIONS_DIR).c_str(), 0755);
	std::string path = getUserPath(username);
	JSON::Dict toSave = user;
	toSave.save(path.c_str());
}

/**
 * Method saving a players data
 * @param Player : player whose data will be saved
 */
void MemoryAccess::save(Player& player){
	std::string path = getPlayerPath(player.getOwner(), player.getMemberID());
	JSON::Dict toSave = player;
	toSave.save(path.c_str());
}

/**
 * Method saving a sale
 * @param Sale : sale whose data will be saved
 */
void MemoryAccess::save(Sale& sale){
	std::string path = getSalePath(sale.getID());
	JSON::Dict toSave = sale;
	toSave.save(path.c_str());
}

/**
 * Method saving a teams data
 * @param Team : teams whose data will be saved
 */
void MemoryAccess::save(Team& team){
	std::string path = getTeamInfosPath(team.getOwner());
	cout<<"*********Saving team with path: "<<path<<endl;
	JSON::Dict toSave = team;
	toSave.save(path.c_str());
}

/**
 * Method saving a championship data
 * @param Championship : championship whose data will be saved
 */
void MemoryAccess::save(Championship& champ){
	mkdir(memory::CHAMPIONSHIPS_DIR.c_str(), 0755);
	std::string path = getChampionshipPath(champ.getName());
	JSON::Dict toSave = champ;
	toSave.save(path.c_str());
}

/**
 * Method saving multiple items
 * @param std::vector : items to be saved
 * @param std::string : type of the items to be saved
 */
void MemoryAccess::save(std::vector<std::string> &toSave,std::string type){
	std::string path = "";
	if(type == memory::ALL_TEAM_NAMES){
		path = getTeamNamesPath();
	}
	else if (type== memory::ALL_USER_NAMES){
		path = getUserNamesPath();
	}
	JSON::List jsonToSave;
	for(size_t i =0;i<toSave.size();++i){
		jsonToSave.append(toSave[i]);
	}
	jsonToSave.save(path.c_str());
}

/**
 * Method loading player data
 * @param Player : player whose data will be loaded from the HDD
 */
void MemoryAccess::load(Player& player){
	JSON::Value *loaded = JSON::load(getPlayerPath(player.getOwner(),player.getMemberID()).c_str());
	player = DICT(loaded);
	delete loaded;
}

/**
 * Method loading user data
 * @param User : user whose data will be loaded
 */
void MemoryAccess::load(User& user){
	JSON::Value *loaded = JSON::load(getUserPath(user.getUsername()).c_str());
	user = &DICT(loaded); //Constructor by pointer in User ... 
	delete loaded;

}

/**
 * Method loading sale data
 * @param Sale : sale whose data will be loaded from the HDD
 */
void MemoryAccess::load(Sale& sale){
	JSON::Value *loaded = JSON::load(getSalePath(sale.getID()).c_str());
	sale = DICT(loaded);
	delete loaded;
}

/**
 * Method loading installations data
 * @param Installations : installations whose data will be loaded from the HDD
 */
void MemoryAccess::load(Installation* install){
	JSON::Value *loaded = JSON::load(getInstallationPath(install->getOwner(),install->getName()).c_str());
	JSON::Dict const & tmp = DICT(loaded);
	install = Installation::CAST(tmp);
	delete loaded;
}

/**
 * Method loading team data
 * @param Team : team whose data will be loaded from the HDD
 */
void MemoryAccess::load(Team& team){
	JSON::Value *loaded = JSON::load(getTeamInfosPath(team.getOwner()).c_str());
	team = DICT(loaded);
	delete loaded;
}

/**
 * Method loading championship data
 * @param Championship : championship whose data  will be loaded from the HDD
 */
void MemoryAccess::load(Championship& champ){
	JSON::Value *loaded = JSON::load(getChampionshipPath(champ.getName()).c_str());
	champ = DICT(loaded);
	delete loaded;
}


/**
 * Method loading multiple data files in a vector
 * @param std::string : directory from which to load the data
 * @return JSON::List : json object containing the data loaded from the directory
 */
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
				//checks if file is a .json file
				if(str1.compare(str1.size()-str2.size(),str2.size(),str2) == 0){
					JSON::Value* loaded = JSON::load((directory+file->d_name).c_str());
					if(ISDICT(loaded)){
						ret.append(DICT(loaded));
					}
					delete loaded;
				}
			}
		}
	}
	closedir(dir);
	return ret;
}

/**
 * Method loading data for multiple users
 * @param std::Vector : vector of users whose data will be loaded
 */
void MemoryAccess::load(std::vector<User> &toFill)
{
	vector<string> users;
	MemoryAccess::load(users, memory::ALL_USER_NAMES);
	for (size_t i = 0; i < users.size(); ++i)
	{
		User user(users[i]);
		MemoryAccess::load(user);
		toFill.push_back(user);
	}
}

/**
 * Method loading multiple installations
 * @param std::vector : vector containing  the installations of a user
 */
void MemoryAccess::load(std::vector<Installation*> &toFill,std::string username){
	JSON::List installs = loadFilesInVec(getInstallationsDirectory(username));
	for(size_t i=0;i<installs.len();++i){
		toFill.push_back(Installation::CAST(DICT(installs[i])));
	}
}

/**
 * Method loading multiple players
 * @param std::vector : vector of players whose data will be loaded
 */
void MemoryAccess::load(std::vector<Player> &toFill,std::string username){
	JSON::List players = loadFilesInVec(getPlayersDirectory(username));
	for(size_t i=0;i<players.len();++i){
		toFill.push_back(DICT(players[i]));
	}
}

/**
 * Method loading multiple sales
 * @param std::vector : vector containing the sales whose data will be loaded
 */
void MemoryAccess::load(std::vector<Sale*> &toFill){
	mkdir(memory::MARKET_PATH.c_str(), 0755);
	JSON::List sales = loadFilesInVec(memory::MARKET_PATH);
	for(size_t i=0;i<sales.len();++i){
		toFill.push_back(new Sale(DICT(sales[i])));
	}
}

/**
 * Method loading multiple championships
 * @param std::deque : double ended que containing the championships whose data will be loaded
 */
void MemoryAccess::load(std::deque<Championship*>& toFill){
	mkdir(memory::CHAMPIONSHIPS_DIR.c_str(), 0755);
	JSON::List champs = loadFilesInVec(memory::CHAMPIONSHIPS_DIR);
	for(size_t i=0;i<champs.len();++i){
		toFill.push_back(new Championship(DICT(champs[i])));
	}
}

/**
 * Method loading multiple items
 * @param std::vector : vector containing items to be loaded
 * @param std::string : string containing the type of the items to be loaded
 */
void MemoryAccess::load(std::vector<std::string> &toFill, std::string type){
	std::string path = "";
	if(type == memory::ALL_TEAM_NAMES){
		path = getTeamNamesPath();
	}
	else if (type == memory::ALL_USER_NAMES){
		path = getUserNamesPath();	
	}
	try {
		JSON::Value *loaded = JSON::load(path.c_str());
		JSON::List & names = LIST(loaded);
		for(size_t i=0;i<names.len();++i){
			toFill.push_back(STR(names[i]).value());
		} 
		delete loaded;
	}
	catch(JSON::IOError &e)
	{}
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

void MemoryAccess::loadSkel(std::vector<Installation*> &vec){
	JSON::Value *loaded = JSON::load(getSkelPath(memory::INSTS_SKEL_FILE).c_str());
	JSON::List & insts = LIST(loaded);
	for(size_t i = 0;i<insts.len();++i){
		vec.push_back(Installation::CAST(DICT(insts[i])));
	}
	delete loaded;
}

void MemoryAccess::saveAdmin(User& admin){
	std::string path = getAdminPath(admin.getUsername());
	mkdir(memory::ADMIN_DIR.c_str(), 0755);
	mkdir((memory::ADMIN_DIR+admin.getUsername()).c_str(), 0755);
	JSON::Dict toSave = admin;
	toSave.save(path.c_str());
}

/**
 * Method loading admin credentials from server db
 * @param User : user whose credentials have to be loaded
 */
void MemoryAccess::loadAdmin(User& admin){
	JSON::Value *loaded = JSON::load(getAdminPath(admin.getUsername()).c_str());
	admin = &DICT(loaded); //Constructor by pointer in User ... 
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

void MemoryAccess::removeObject(Championship &champ){
	remove(getChampionshipPath(champ.getName()).c_str());
}

void MemoryAccess::removeObject(Installation* install){
	remove(getInstallationPath(install->getOwner(),install->getName()).c_str());
}
