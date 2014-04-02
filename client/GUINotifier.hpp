#ifndef DEFINE_GUINOTIFIER_HELPER
#define DEFINE_GUINOTIFIER_HELPER

#include <match/GraphicMatchView.hpp>
#include <graphics/MessageBox.hpp>
#include "graphics/Bank.hpp"

/*! Graphical module mixin that allows automatic handling of notifications and
    incoming messages in main event loop */
#define NOTIFIABLE \
virtual void checkNotifications(){readMessages();handleNotification();}\
virtual void launchMatch(){GraphicMatchView match(*this, _controller);match.run();redrawCanvas();}\
virtual void onOtherAccept(std::string const & user){sf::Sound sound(SOUNDS.get("invite_accept.wav")); sound.play(); launchMatch();}\
virtual void onOtherDeny(std::string const & user){displayError(user + " doesn't want to play with you");}\
virtual void onInvite(std::string const & otherUser){sf::Sound sound(SOUNDS.get("invite.wav")); sound.play(); std::string question = otherUser + " has invited you to play a friendly game.\nAccept ?";if (confirm(question)){acceptInvitationFromUser(otherUser);launchMatch();} else {denyInvitationFromUser(otherUser);}}\
virtual void onMessage(std::string const & message){displayOk(message);}\
virtual void onNotificationResponse(bool success, std::string const & response, std::string const & message){(success) ? displayOk(message) : displayError(message);}\
virtual void onMatchPending(){MessageBox m(_controller, "Next championship match is close. What do you want to do ?", {"Ready to play !", "Withdraw"});int res = m.showBox();(res==0) ? readyForMatch() : withdrawFromMatch();}\
virtual void onMatchStart(){launchMatch();}\
virtual void onTeamInfoChange(){updateUsername(user().username);updateBudget(user().funds);updateFame(user().fame);updateAcPoints(user().acPoints);redrawCanvas();}
#endif
