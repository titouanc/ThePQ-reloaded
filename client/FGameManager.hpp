#ifndef DEFINE_FGAMEMANAGER_HEADER
#define DEFINE_FGAMEMANAGER_HEADER

#include "ClientManager.hpp"

/* Friendly game manager */
class FGameManager : public ClientManager {
	protected:
		void askConnectedList();
		void sendInvitation(std::string const & name);
	public:
		using ClientManager::ClientManager;
		FGameManager(ClientManager const & parent);
		void treatMessage(JSON::Dict const & msg);

		/* === HOOKS === */
		/* User list received */
		virtual void onUserList(){}
		/* Other user accepted invitation */
		virtual void onOtherAccept(std::string const & name){}
		/* Other user denied invitation */
		virtual void onOtherDeny(std::string const & name){}
		/* Other user not found */
		virtual void onUserNotFound(std::string const & name){}
};

#endif