#ifndef DEFINE_GUINOTIFIER_HELPER
#define DEFINE_GUINOTIFIER_HELPER

#include <match/GraphicMatchManager.hpp>

/*! Base graphical Manager that can spawn notifications */

#define NOTIFIABLE \
virtual void loop(){readMessages();handleNotification();}\
virtual void launchMatch(){GraphicMatchManager match(*this, _controller);match.run();redrawCanvas();}\
virtual void onOtherAccept(std::string const & user){launchMatch();}\
virtual void onOtherDeny(std::string const & user){displayError(user + " doesn't want to play with you");}\
virtual void onInvite(std::string const & otherUser){std::string question = otherUser + " has invited you to play a friendly game.\nAccept ?";if (confirm(question)){acceptInvitationFromUser(otherUser);launchMatch();} else {denyInvitationFromUser(otherUser);}}\
virtual void onMessage(std::string const & message){displayMessage(message);}

#endif
