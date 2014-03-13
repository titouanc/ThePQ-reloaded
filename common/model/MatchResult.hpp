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

	void setTeams(std::string winner, std::string loser)
	{
		_winner.setOwner(winner); _loser.setOwner(loser);
		_winner.loadInfos(); _loser.loadInfos();
	}

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

	void compute(bool isChampMatch = false, int turn = 0, int totalturn = 0, int cashprize = 0)
	{
		resolveFame();	
		resolveMoney(isChampMatch,turn,totalturn,cashprize);
		resolveAP();
	}

	void resolveMoney(bool isChampMatch = false, int turn = 0, int totalturn = 0, int cashprize = 0){
		if(!isChampMatch){
			_winnerMoneyGain = int(gameconfig::FUNDS_EARN_GAME*gameconfig::FUNDS_GAME_RATIO);
		}
		else{
			_winnerMoneyGain = int(gameconfig::FUNDS_EARN_GAME*gameconfig::FUNDS_CHAMP_RATIO + cashprize/(totalturn-turn+2));
		}
	}

	void resolveFame(){
		/*
		 * Method calculating the fame to be attributed to each player
		 * based on the existing fame of the teams and the score difference
		 */

		int wFame = _winner.getFame();
		int lFame = _loser.getFame();
		//Case 1 : winner's fame >= loser's fame, fame lose/win isn't that much
		if (wFame>=lFame){
			if ((_score[0] - _score[1])>=gameconfig::BIG_GOAL_DIFFERENCE){
				_winnerFameGain = (int((gameconfig::FAME_WIN/(wFame-lFame))*0.15));
				_loserFameGain = -(int((gameconfig::FAME_WIN/(wFame-lFame))*0.15));
			}
			else{
				_winnerFameGain = (int((gameconfig::FAME_WIN/(wFame-lFame))*0.05));
				_loserFameGain = -(int((gameconfig::FAME_WIN/(wFame-lFame))*0.05));
			}
		}
		//Case 2 : winner's fame < loser's fame , fame lose/win more important
		else if(wFame<lFame){
			if ((_score[0] - _score[1])>=gameconfig::BIG_GOAL_DIFFERENCE){
				_winnerFameGain = (int((gameconfig::FAME_WIN*(wFame-lFame))*0.15));
				_loserFameGain = -(int((gameconfig::FAME_WIN*(wFame-lFame))*0.15));
			}
			else{
				_winnerFameGain = (int((gameconfig::FAME_WIN*(wFame-lFame))*0.05));
				_loserFameGain = -(int((gameconfig::FAME_WIN*(wFame-lFame))*0.05));
			}
		} 		
	}

	void resolveAP(){
		_winnerAPGain = gameconfig::MATCH_WON_AP_GAIN;
		_loserAPGain = gameconfig::MATCH_LOST_AP_GAIN;
	}

	void resolveFameDisconnection(std::string win){
		_winnerFameGain = gameconfig::FAME_EARN_DISCONNECT;
	}

	void resolveMoneyDisconnection(std::string win){
		_winnerMoneyGain = gameconfig::FUNDS_EARN_DISCONNECT;	
	}

	std::string getWinner() const { return _winner.getOwner(); }
	std::string getLoser() const { return _loser.getOwner(); }
	bool isSaved() const { return _saved; }
	void saved() {_saved = true;}
	unsigned int* getScore() { return _score; }
	int getWinnerFameGain() const { return _winnerFameGain; }
	int getLoserFameGain() const { return _loserFameGain; }
	int getWinnerMoneyGain() const { return _winnerMoneyGain; }
	int getLoserMoneyGain() const { return _loserMoneyGain; }
	int getWinnerAPGain() const { return _winnerAPGain; }
	int getLoserAPGain() const { return _loserAPGain; }

private:
	Team _winner, _loser;
	unsigned int _score[2];
	int _winnerFameGain, _loserFameGain;
	int _winnerMoneyGain, _loserMoneyGain;
	int _winnerAPGain, _loserAPGain;
	bool _saved;
};

#endif // __MATCH_RESULT_HPP