#ifndef __STADIUM_MANAGER_HPP
#define __STADIUM_MANAGER_HPP

#include <vector>
#include <ClientController.hpp>
#include <cli/Menu.hpp>
#include <model/Installation.hpp>

class StadiumController : public ClientController
{
protected:
	/* attempt to upgrade an installation */
	void upgradeInstallation(size_t i);
	/* attempt to downgrade an installation */
	void downgradeInstallation(size_t i);
	/* ask for installations list */
	void loadInstallations();
	/* treat Stadium specific messages */
	void treatMessage(std::string const & type, JSON::Value const * data);
	/* HOOKS */
	/* received an installation upgrade response */
	virtual void onUpgradeInstallation(bool);
	/* received an installation downgrade response */
	virtual void onDowngradeInstallation(bool);
	/* received installations list : put it in user().installations */
	virtual void onInstallationsLoad(){}
public:
	using ClientController::ClientController;
	StadiumController(ClientController const & parent);
};

#endif // __STADIUM_MANAGER_HPP
