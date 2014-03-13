#ifndef __MATCH_RESULT_HPP
#define __MATCH_RESULT_HPP

#include <string>
#include <model/Team.hpp>
#include <Constants.hpp>

class MatchResult
{
public:
	MatchResult() : 
		_winner(), _loser(), _score(), _fameDiff(), _moneyGain(), _APGain(), _saved(false)
	{}

	void setTeams(std::string winner, std::string loser)
	{
		_winner = winner;
		_loser = loser;
	}

	void setScore(unsigned int home, unsigned int away)
	{
		_score[0] = home;
		_score[1] = away;
	}

	void compute(bool c)
	{
		Team winner(_winner), looser(_loser);
		winner.load();
		looser.load();
		resolveFame(winner,looser);	
		resolveMoney(winner,looser,c);
	}

	void save()
	{
		if (_saved == false)
		{
			Team winner(_winner), loser(_loser);
			winner.loadInfos();
			winner.earnFame(_fameDiff);
			winner.getPayed(_moneyGain[0]);
			winner.earnAcPoints(_APGain[0]);
			loser.loadInfos();
			loser.loseFame(_fameDiff);
			loser.getPayed(_moneyGain[1]);
			loser.earnAcPoints(_APGain[1]);
			loser.save();
			winner.save();
			_saved = true;
		}
	}
	void resolveMoney(Team &winner,Team &looser,bool champMatch){
		if(champMatch){
			winner.getPayed(looser.loseFunds(int(gameconfig::FUNDS_EARN_GAME*gameconfig::FUNDS_GAME_RATIO*looser.getFame())));
		}else{
			winner.getPayed(looser.loseFunds(int(gameconfig::FUNDS_EARN_GAME*gameconfig::FUNDS_CHAMP_RATIO*looser.getFame())));
		}
		winner.save();
		looser.save();
		
	}
	void resolveFame(Team &winner,Team &looser){
		/*Method calculating the fame to be attributed to each player
		 *based on the existing fame of the teams and the score difference
		 */
		int wFame = winner.getFame();
		int lFame = looser.getFame();
		if (wFame>lFame){//compare fame between teams
			if ((_score[0] - _score[1])>10){//big goal difference
				winner.earnFame(int((wFame-lFame)*0.45));
				looser.loseFame(int((wFame-lFame)*0.45));
			}else{
				winner.earnFame(int((wFame-lFame)*0.15));
				looser.loseFame(int((wFame-lFame)*0.15));
			}
		}else if(wFame<lFame){
			if ((_score[0] - _score[1])>10){
				winner.earnFame(int((lFame-wFame)*0.75));
				looser.loseFame(int((lFame-wFame)*0.75));
			}else{
				winner.earnFame(int((lFame-wFame)*0.60));
				looser.loseFame(int((lFame-wFame)*0.60));
			}

		}else{
			if ((_score[0] - _score[1])>10){
				winner.earnFame(int(lFame*0.45));
				looser.loseFame(int(lFame*0.45));
			}else{
				winner.earnFame(int(lFame*0.15));
				looser.loseFame(int(lFame*0.15));
			}		
		}
		looser.save();
		winner.save();
	}
	void resolveFameDisconnection(std::string win){
		Team winner(win);
		winner.loadInfos();
		winner.earnFame(gameconfig::FAME_EARN_DISCONNECT);
		winner.save();
	}

	void resolveMoneyDisconnection(std::string win){
		Team winner(win);
		winner.loadInfos();
		winner.getPayed(gameconfig::FUNDS_EARN_DISCONNECT);
		winner.save();
	}

	std::string getWinner() { return _winner; }
	std::string getLoser() { return _loser; }

private:
	std::string _winner;
	std::string _loser;
	unsigned int _score[2];
	unsigned int _fameDiff;
	unsigned int _moneyGain[2];
	unsigned int _APGain[2];
	bool _saved;
};

#endif // __MATCH_RESULT_HPP