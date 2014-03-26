#ifndef __CHAMPIONSHIP_MANAGER_HPP
#define __CHAMPIONSHIP_MANAGER_HPP

#include "GameManager.hpp"
#include <model/Championship.hpp>

class ChampionshipManager : public GameManager
{
protected:
    std::deque<Championship*>& _championships;
    std::deque<Schedule>& _pendingChampMatches;
    pthread_mutex_t& _champsMutex; //_championships used by 3 threads
public:
	using GameManager::GameManager;
	ChampionshipManager(const GameManager & parent,
						std::deque<Championship*>& championships,
						std::deque<Schedule>& pendingChampMatches,
						pthread_mutex_t& champsMutex) : 
						GameManager(parent), _championships(championships),
						_pendingChampMatches(pendingChampMatches),
						_champsMutex(champsMutex)
	{}

    Championship* getChampionshipByName(std::string champName);
    Championship* getChampionshipByUsername(std::string username);
    size_t nbrUsersConnected(){return _users.size();}
    void loadChampionships();
    void leaveChampionship(int);
    void joinChampionship(std::string,int);
    void sendChampionshipsList(int);
    void sendJoinedChampionship(int);
    void notifyPendingChampMatch(std::string);
    void notifyStartingChampionship(Championship&);
    void responsePendingChampMatch(std::string,int);
    void resolveUnplayedChampMatch(Schedule&);
    void endOfPending(Schedule&);
    Schedule* getPendingMatchByUsername(std::string);
};

#endif // __CHAMPIONSHIP_MANAGER_HPP