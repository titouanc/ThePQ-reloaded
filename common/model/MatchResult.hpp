#ifndef __MATCH_RESULT_HPP
#define __MATCH_RESULT_HPP

#include <string>
#include <model/Team.hpp>
#include <Constants.hpp>

class MatchResult
{
public:
	MatchResult() : 
		_winner(), _loser(), _score(), _winnerFameGain(0), _loserFameGain(0), 
		_winnerMoneyGain(0),_loserMoneyGain(0), _winnerAPGain(0), _loserAPGain(0), _saved(false)
	{}

	/**
	 * Method setting the winning/loosing teams
	 * @param std::string name of the winning team
	 * @param std::string name of the loosing team
	 */
	void setTeams(std::string winner, std::string loser)
	{
		_winner.setOwner(winner); _loser.setOwner(loser);
		_winner.loadInfos(); _loser.loadInfos();
	}

	/** Method setting the score of a match
	 * @param int : score of the winner
	 * @param int : score of the looser
	 */
	void setScore(unsigned int score1, unsigned int score2)
	{
		if(score1 >= score2){
			_score[0] = score1;
			_score[1] = score2;
		}
		else{
			_score[0] = score2;
			_score[1] = score1;
		}
	}

	/// Method setting up the host for the connection
	void setHost(std::string host)
	{
		_host = host;
	}

	/**
     * Method computing the fame the money and activity points after a match
     * @param bool if compute for a championship match or friendly match
     * @param int  turn at which the game ended(if a championship game)
     * @param int total turns of the championship (if a championship game)
     * @param int cashprize of the championship (if championship game)
	 */
	void compute(bool isChampMatch = false, int turn = 0, int totalturn = 0, int cashprize = 0)
	{
		resolveFame();	
		resolveMoney(isChampMatch,turn,totalturn,cashprize);
		resolveAP();
	}

	/**
 	 * Method computing the money distribution among the users
 	 * @param bool if compute for a championship match or friendly match
     * @param int  turn at which the game ended(if a championship game)
     * @param int total turns of the championship (if a championship game)
     * @param int cashprize of the championship (if championship game)
     */
	void resolveMoney(bool isChampMatch = false, int turn = 0, int totalturn = 0, int cashprize = 0){
		if(!isChampMatch){
			_winnerMoneyGain = int(gameconfig::FUNDS_EARN_GAME*gameconfig::FUNDS_GAME_RATIO);
		}
		else{
			_winnerMoneyGain = int(gameconfig::FUNDS_EARN_GAME*gameconfig::FUNDS_CHAMP_RATIO + cashprize/(totalturn-turn+2));
		}
		Team host(_host);
		host.load();
		int maxSpectators = host.getTribune()->getMaxSpectators();
		int moneyGain = 0;
		for (Installation* install : host.getInstallations())
		{
			moneyGain += install->getMatchIncome(maxSpectators);
		}
		if (_host == _winner.getName())
		{
			_winnerMoneyGain += moneyGain;
		}
		else
		{
			_loserMoneyGain = moneyGain;
		}
	}

	/**
	 * Method calculating the fame to be attributed to each player
	 * based on the existing fame of the teams and the score difference
	 */
	void resolveFame(){
		

		int wFame = _winner.getFame();
		int lFame = _loser.getFame();
		double fameRatio = lFame/wFame;
		double goalRatio = ((_score[0] - _score[1] >= gameconfig::BIG_GOAL_DIFFERENCE) ? 1.5 : 1.0);
		int fameGain = fameRatio*goalRatio*gameconfig::FAME_WIN;
		_winnerFameGain = fameGain;
		_loserFameGain = -fameGain; 		
	}

	/**
     * Method computing the activity points allowance for the users
	 */
	void resolveAP(){
		_winnerAPGain = gameconfig::MATCH_WON_AP_GAIN;
		_loserAPGain = gameconfig::MATCH_LOST_AP_GAIN;
	}

	// void resolveFameDisconnection(std::string win){
	// 	_winnerFameGain = gameconfig::FAME_EARN_DISCONNECT;
	// }

	// void resolveMoneyDisconnection(std::string win){
	// 	_winnerMoneyGain = gameconfig::FUNDS_EARN_DISCONNECT;	
	// }

	/// Method retrieving the winner of a game
	std::string getWinner() const { return _winner.getOwner(); }
	
	/// Method retrieving the looser of a game
	std::string getLoser() const { return _loser.getOwner(); }

	/// Method cheking if a game result is saved
	bool isSaved() const { return _saved; }

	/// Method setting the flags for saved earnings
	void saved() {_saved = true;}

	/// Method retrieving the score of a game
	unsigned int* getScore() { return _score; }

	/// Method retrieving the fame earned by the winner
	int getWinnerFameGain() const { return _winnerFameGain; }

	/// Method retrieving the fame earned by the looser
	int getLoserFameGain() const { return _loserFameGain; }
	
	/// Method retrieving the money earned by the winner
	int getWinnerMoneyGain() const { return _winnerMoneyGain; }

	/// Method retrieving the money earned by the looser
	int getLoserMoneyGain() const { return _loserMoneyGain; }

	/// Method retrieving the activity points earned by the winner
	int getWinnerAPGain() const { return _winnerAPGain; }

	/// Method retrieving the activity points earned by the looser
	int getLoserAPGain() const { return _loserAPGain; }

private:
	Team _winner, _loser;
	unsigned int _score[2];
	int _winnerFameGain, _loserFameGain;
	int _winnerMoneyGain, _loserMoneyGain;
	int _winnerAPGain, _loserAPGain;
	bool _saved;
	std::string _host;
};

#endif // __MATCH_RESULT_HPP