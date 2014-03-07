#ifndef DEFINE_FGAMEMANAGER_HEADER
#define DEFINE_FGAMEMANAGER_HEADER

#include "ClientManager.hpp"

/* Friendly game manager */
class FGameManager : public ClientManager {
	protected:
		void askConnectedList();
		void sendInvitation(std::string const & name);
		void acceptInvitationFromUser(string username);
		void denyInvitationFromUser(string username);
	public:
		using ClientManager::ClientManager;
		FGameManager(ClientManager const & parent);
		void treatMessage(std::string const & type, JSON::Value const * data);

		/* === HOOKS === */
		/* User list received */
		virtual void onUserList(JSON::List const &){}
		/* Other user accepted invitation */
		virtual void onOtherAccept(std::string const &){}
		/* Other user denied invitation */
		virtual void onOtherDeny(std::string const &){}
		/* Other user not found */
		virtual void onUserNotFound(std::string const &){}
};

#endif