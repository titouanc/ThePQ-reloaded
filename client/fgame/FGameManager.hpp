#ifndef DEFINE_FGAMEMANAGER_HEADER
#define DEFINE_FGAMEMANAGER_HEADER

#include <ClientManager.hpp>

/* Friendly game manager */
class FGameManager : public ClientManager {
	protected:
		/* Asks for connected users list */
		void askConnectedList();
		/* Send invitation to a friendly game to another user */
		void sendInvitation(std::string const & name);
		/* Accept invitation of another user to a friendly game */
		void acceptInvitationFromUser(string username);
		/* Deny invitation of another user to a friendly game */
		void denyInvitationFromUser(string username);
		/* Treat Friendly Game specfic messages from the server */
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
	public:
		using ClientManager::ClientManager;
		FGameManager(ClientManager const & parent);
};

#endif