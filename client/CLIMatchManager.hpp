#include "MatchManager.hpp"
#include "Menu.hpp"

class CLIMatchManager : public MatchManager {
	private:
	public:
		void askUser(){
			cout << "Choose a player to move: " << endl;
			Menu menu;
			for (int i=0; i<7; i++){
				Player const & p = *(mySquad().players[i]);
				menu.addToDisplay(colorPlayerLetter(p)+" "+p.getName());
			}
		}

		void run(){
			cout << "CLIMatchManager started" << endl;
			while (state() != FINISHED){
				while (connection().hasMessage()){
					JSON::Value *msg = connection().popMessage();
					treatMessage(DICT(msg));
					delete msg;
				}

				displayPitch();
				askUser();
			}
			cout << "CLIMatchManager ended" << endl;
		}

		void onStateChange(){
			if (state() == PROMPT)
				cout << "You can play now !" << endl;
			else if (state() == TIMEOUT)
				cout << "You're play time is finished." << endl;
		}

		void onPitchChange(){
			displayPitch();
		}

		/* Return colored letter for a player (without cleanup escape code) */
		std::string colorPlayerLetter(Player const & player){
			char res[7] = "\033[30mX";
			if (player.isSeeker())
				res[3] = '3';
			else if (player.isChaser())
				res[3] = '4';
			else if (player.isBeater())
				res[3] = '1';
			else if (player.isKeeper())
				res[3] = '6';
			res[5] = 'A' + player.getID() - 1;
			return std::string(res);
		}

		std::string colorBallLetter(Ball const & ball){
			char res[7] = "\033[30m*";
			if (ball.isGoldenSnitch())
				res[3] = '3';
			else if (ball.isQuaffle())
				res[3] = '4';
			else if (ball.isBludger())
				res[3] = '1';
			return std::string(res);
		}

		void displayPitch(){
			Moveable *atPos = NULL;
			for (int y=pitch().ymax()-1; y>=pitch().ymin(); y--){
				for (int x=pitch().xmin(); x<pitch().xmax(); x++){

					/* background */
					if (pitch().isInKeeperZone(x, y))
						cout << "\033[47m";
					else if (pitch().inEllipsis(x, y))
						cout << "\033[42m";

					/* foreground */
					if (! pitch().isValid(x, y) || ! pitch().inEllipsis(x, y)){
						cout << ' ';
					} else {
						atPos = pitch().getAt(x, y);
						if (pitch().isGoal(x, y)){
							cout << "\033[1m\033[44mO";
						} else if (atPos==NULL){
							cout << '.';
						} else if (atPos->isBall()){
							/* Colorize balls by type */
							Ball const & ball = (Ball const &) *atPos;
							cout << colorBallLetter(ball);
						} else if (atPos->isPlayer()){
							/* Colorize players by type */
							Player const & player = (Player const &) *atPos;
							
							if (_matchManager.isOwnPlayer(player))
								cout << "\033[1m";
							cout << colorPlayerLetter(player);
						}
					}
					cout << "\033[0m";
				}
				cout << endl;
			}
			cout << "\033[1m"
				 << "\033[34mChaser \033[31mBeater \033[36mKeeper \033[33mSeeker"
				 << "\033[0m :: \033[1m"
			     << "\033[34m*Quaffle \033[31m*Bludger \033[33m*Golden Snitch\033[0m"
				 << endl;
		}
};