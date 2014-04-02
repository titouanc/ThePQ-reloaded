#ifndef DEFINE_FGAMEMANAGER_HEADER
#define DEFINE_FGAMEMANAGER_HEADER

#include <ClientController.hpp>

/* Friendly game manager */
class FGameManager : public ClientController {
	protected:
		/* Asks for connected users list */
		void askConnectedList();
		/* Send invitation to a friendly game to another user */
		void sendInvitation(std::string const & name);
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
		using ClientController::ClientController;
		FGameManager(ClientController const & parent);
};

#endif