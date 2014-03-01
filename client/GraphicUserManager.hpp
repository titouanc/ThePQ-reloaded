#include <network/ClientConnectionManager.hpp>
#include "graphics/GraphicManager.hpp"
#include "UserManager.hpp"
#include "Exception.hpp"
#include "UserData.hpp"

class GraphicUserManager : public UserManager, public GUI::GraphicManager 
{
public:
	GraphicUserManager(net::ClientConnectionManager& connection, UserData& user, GUI::MainController &controller);
	~GraphicUserManager(){}
private:
	void submitLoginForm();
	GUI::Button<GraphicUserManager> *_submitButton;
	GUI::Textbox *_usernameTextbox;
	GUI::Textbox *_passwordTextbox;
};