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

	void setScore(unsigned int home, unsigned int outdoor)
	{
		_score[0] = home;
		_score[1] = outdoor;
	}

	void compute()
	{
		Team winner(_winner), loser(_loser);
		winner.load();
		loser.load();
		double fameRatio = loser.getFame()/winner.getFame();
		_fameDiff = fameRatio*gameconfig::FAME_GAIN_RATIO;
		// TODO installations
		_moneyGain[0] = fameRatio*gameconfig::MONEY_GAIN_RATIO;
		_moneyGain[1] = fameRatio*gameconfig::MONEY_GAIN_RATIO/gameconfig::WINNER_LOSER_GAIN_RATIO;
		_APGain[0] = fameRatio*gameconfig::AP_GAIN_RATIO;
		_APGain[1] = fameRatio*gameconfig::AP_GAIN_RATIO/gameconfig::WINNER_LOSER_GAIN_RATIO;
	}

	void save()
	{
		if (_saved == false)
		{
			Team winner(_winner), loser(_loser);
			winner.loadInfos();
			winner.addFame(_fameDiff);
			winner.getPayed(_moneyGain[0]);
			winner.addAP(_APGain[0]);
			loser.loadInfos();
			loser.substractFame(_fameDiff);
			loser.getPayed(_moneyGain[1]);
			loser.addAP(_APGain[1]);
			loser.save();
			winner.save();
			_saved = true;
		}
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