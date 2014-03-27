#include "UserManager.hpp"

void UserManager::registerUser(const JSON::Dict &credentials, int peer_id)
{
	if (ISSTR(credentials.get(MSG::USERNAME)) && ISSTR(credentials.get(MSG::PASSWORD))){
		std::string const & username = STR(credentials.get(MSG::USERNAME));
		std::string const & password = STR(credentials.get(MSG::PASSWORD));
		JSON::Dict response = JSON::Dict();
		response.set("type", MSG::REGISTER);
		User* newUser = User::load(username);
		if (newUser != NULL){
			response.set("data", MSG::USER_EXISTS);
		} else { // User doesnt exist
			newUser = new User(username, password);
			newUser->createUser(); 
			response.set("data", MSG::USER_REGISTERED);
		}

		/* Clone dict before sending, because
		     + it should be dynamically allocated (ConnectionManager deletes it)
		     + cloning at the end is less memory efficient, but do not leak
		       memory if a trap/exc occurs after response allocation.
		 */
		Message status(peer_id, response.clone());
		_outbox.push(status);
		delete newUser;
	}
}

User *UserManager::logUserIn(const JSON::Dict &credentials, int peer_id)
{
	User *res = NULL;
	if (ISSTR(credentials.get(MSG::USERNAME)) && ISSTR(credentials.get(MSG::PASSWORD))){
		std::string const & username = STR(credentials.get(MSG::USERNAME));
		std::string const & password = STR(credentials.get(MSG::PASSWORD));

		JSON::Dict response;
		response.set("type", MSG::LOGIN);
		if (getUserByName(username)){
			response.set("data", MSG::ALREADY_LOGGED_IN);
		} else {
			User *user = User::load(username);
			if (user != NULL){
				if (user->getPassword() == password){
					// correct password
					// load the user's team info into Team (installations, players, funds, etc.)
					user->loadTeam();
					// mapping user to its peer_id to keep a list of connected users.
					_users.insert(std::pair<int, User*>(peer_id, user));
					if(user->getTeam().getName() == gameconfig::UNNAMED_TEAM)
						response.set("data",MSG::USER_CHOOSE_TEAMNAME);
					else
					{
						response.set("data", MSG::USER_LOGIN);
					}
					res = user;
				} else {
					// wrong password
					delete user;
					response.set("data", MSG::PASSWORD_ERROR);
				}
			} else {
				// user not found
				response.set("data", MSG::USER_NOT_FOUND);
			}
		}
		_outbox.push(Message(peer_id, response.clone()));
	}
	return res;
}

void UserManager::checkTeamName(const JSON::Dict &data, int peer_id){
	std::vector<std::string> teamNames;
	std::string teamname = STR(data.get(net::MSG::TEAMNAME)).value();
	bool exists = false;
	try{
		MemoryAccess::load(teamNames,memory::ALL_TEAM_NAMES);
	}
	catch(JSON::IOError e){}
	for(size_t i =0;i<teamNames.size();++i){
		if(teamNames[i]==teamname)
			exists = true;
	}
	JSON::Dict response;
	response.set("type", MSG::TEAMNAME);
	if(!exists){
		teamNames.push_back(teamname);
		MemoryAccess::save(teamNames,memory::ALL_TEAM_NAMES);
		Team & team = _users[peer_id]->getTeam();
		team.setName(teamname);
		team.saveInfos();
		response.set("data",MSG::TEAMNAME_REGISTERED);
	}
	else{
		response.set("data",MSG::TEAMNAME_ERROR);
	}
	Message status(peer_id, response.clone());
	_outbox.push(status);

}