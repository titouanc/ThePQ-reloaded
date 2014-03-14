#ifndef __CONSTANTS_HPP
#define __CONSTANTS_HPP
#include <string>
namespace net
{
	// NETWORK MESSAGES --------------------------------------------------------
	namespace MSG
	{
		// TYPE ----------------------------------------------
		static const std::string SERVER_RESPONSE = "S_R";
		static const std::string DISCONNECT = "D";
		static const std::string LOGIN = "Q_L";
		static const std::string REGISTER = "Q_R";
		static const std::string USER_EXISTS = "U_E";
		static const std::string STATUS = "Q_S";
		static const std::string INSTALLATION_UPGRADE = "AQ_I_U";
		static const std::string INSTALLATION_DOWNGRADE = "AQ_I_D";
		static const std::string TEAM_INFOS = "T_I";

		static const std::string ADD_PLAYER_ON_MARKET_QUERY = "Q_A_P_OM";	
		static const std::string DELETE_PLAYER_OF_MARKET_QUERY = "Q_D_P_OM";
		static const std::string BID_ON_PLAYER_QUERY = "Q_B_OP"; 			

		// DATA ----------------------------------------------
		// connection + register
		static const std::string USERNAME = "D_U";
		static const std::string PASSWORD = "D_P";
		static const std::string TEAMNAME = "teamname";
		static const std::string FUNDS = "D_F";
		static const std::string USER_LOGIN = "U_L";
		static const std::string USER_CHOOSE_TEAMNAME = "U_CTN";
		static const std::string TEAMNAME_REGISTERED = "U_TNR";
		static const std::string TEAMNAME_ERROR = "U_TNE";
		static const std::string PASSWORD_ERROR = "U_P";
		static const std::string USER_NOT_FOUND = "U_NF";
		static const std::string USER_REGISTERED = "U_R";
		static const std::string ALREADY_LOGGED_IN = "U_ALI";

		static const std::string PLAYER = "P_DICT"; 		//JSON::Dict of players
		static const std::string PLAYER_ID = "memberID";						
		static const std::string TEAM_ID = "T_ID";			//modif				
		static const std::string BID_VALUE = "B_V";
		static const std::string NEXT_BID = "N_B";
		static const std::string BID_TIMER = "B_T";
		static const std::string TEAMS_BIDDING = "TS_B";
		static const std::string CAN_BID_TEAMS = "C_B_T";
		static const std::string TURN_NUMBER = "T_N";
		static const std::string CURRENT_BIDDER = "C_B";
		static const std::string SALE_OWNER = "S_OWN";
		static const std::string SALE_STATUS = "S_ST";

		//Market
		static const std::string PLAYER_ALREADY_ON_MARKET = "PM_P_A_OM";	
		static const std::string PLAYER_ADDED_ON_MARKET = "PM_PA_OM";		
		static const std::string PLAYER_DELETED_OF_MARKET = "PM_D_OM";		
		static const std::string PLAYER_NOT_ON_MARKET = "PM_N_OM";			
		static const std::string NOT_YOUR_SALE = "PM_N_YS";					
		static const std::string CANNOT_BID_ON_YOUR_PLAYER = "PM_N_BYP";	
		static const std::string BID_TURN_ERROR = "PM_B_D_E";				
		static const std::string LAST_BIDDER = "PM_LB";						
		static const std::string BID_PLACED = "PM_BP";						
		static const std::string BID_ENDED = "PM_BE";						
		static const std::string BID_VALUE_NOT_UPDATED = "PM_B_V_NU";
		static const std::string MARKET_MESSAGE = "PM_MSG";
		static const std::string END_OF_OWNED_SALE_RAPPORT = "PM_EOOS_R";
		static const std::string WON_SALE_RAPPORT = "PM_WSR";
		static const std::string RAPPORT_SALE_STATUS = "PM_R_SS";
		static const std::string PLAYER_NOT_SOLD = "PM_P_N_S";
		static const std::string PLAYER_SOLD = "PM_P_S";
		static const std::string TOO_MANY_PLAYERS = "T_M_P";
		static const std::string NOT_ENOUGH_PLAYERS = "N_E_P";
		static const std::string INSUFFICIENT_FUNDS = "I_CASH";
		static const std::string GENERATED_BY_MARKET = "GEN_B_M";
		// data queries
		static const std::string INSTALLATIONS_LIST = "DQ_I_L";
		static const std::string CONNECTED_USERS_LIST = "DQ_CU_L";
		static const std::string PLAYERS_ON_MARKET_LIST= "DQ_P_OM_L";
		static const std::string PLAYERS_LIST = "DQ_P_L";
		static const std::string SHOW_TEAM_STATUS = "DQ_T_S";//new data query!!!
		static const std::string SHOW_FINANCIAL_STATUS = "DQ_F_S";

		// friendly game
		static const std::string FRIENDLY_GAME_USERNAME = "F_G_U";
		static const std::string FRIENDLY_GAME_INVITATION = "F_G_I";
		static const std::string FRIENDLY_GAME_INVITATION_RESPONSE = "F_G_I_R";
		static const std::string FRIENDLY_GAME_INVITATION_ACCEPT = "F_G_I_A";
		static const std::string FRIENDLY_GAME_INVITATION_DENY = "F_G_I_D";

		// MATCH ----------------------------------------------------------------
		static const std::string MATCH_START     = "MSTART";
		static const std::string MATCH_END       = "MEND";
		static const std::string MATCH_MOVEABLES = "MMOV";
		static const std::string MATCH_PROMPT    = "M?";
		static const std::string MATCH_TIMEOUT   = "MTOUT";
		static const std::string MATCH_STROKE    = "MSTROKE";
		static const std::string MATCH_ERROR     = "M!!!";
		static const std::string MATCH_ACK       = "MACK";
		static const std::string MATCH_DELTA     = "MDELTA";
		static const std::string MATCH_SCORES    = "MSCORES";
		static const std::string MATCH_STATUS    = "MSTATUS";
		
		//Admin------------------
		static const std::string ADMIN_CLIENT = "ADMIN";
		static const std::string ADMIN_LOGIN = "ALI";
		static const std::string CHAMPIONSHIP_CREATION = "ACSHIPC";
		static const std::string CHAMPIONSHIP_ALREADY_EXISTS = "C_name_E";
		static const std::string CHAMPIONSHIP_CREATED = "C_CREATED";

		//Championship------------------
		static const std::string CHAMPIONSHIP_NOTIFICATION = "CHAMP_NOTIF";
		static const std::string TEAMS_NUMBER = "nbOfUsers";
		static const std::string CHAMPIONSHIP_NAME = "name";
		static const std::string CHAMPIONSHIP_CASHPRIZE = "cashprize";
		static const std::string CHAMPIONSHIP_FAME = "fame";
		static const std::string JOINABLE_CHAMPIONSHIPS_LIST = "C_LIST";
		static const std::string JOINED_CHAMPIONSHIP = "C_JOINED";
		static const std::string ALREADY_IN_CHAMPIONSHIP = "C_AIC";
		static const std::string CHAMPIONSHIP_FULL = "C_FULL";
		static const std::string CHAMPIONSHIP_STARTED = "C_START";
		static const std::string NOT_IN_CHAMPIONSHIP = "C_NIC";
		static const std::string LEAVE_CHAMPIONSHIP = "C_LEAVE";
		static const std::string REMOVED_FROM_CHAMPIONSHIP = "C_REMOVED";
		static const std::string JOIN_CHAMPIONSHIP = "C_JOIN";
		static const std::string ADDED_TO_CHAMPIONSHIP = "C_ADDED";
		static const std::string CHAMPIONSHIP_NOT_FOUND = "C_NOTFOUND";
		static const std::string CHAMPIONSHIP_WON = "C_WON";
		static const std::string CHAMPIONSHIP_MATCH_PENDING = "CMATCH_PENDING";
		static const std::string CHAMPIONSHIP_MATCH_STATUS_CHANGE = "CMATCH_SCHANGE";
		static const std::string CHAMPIONSHIP_MATCH_OFFSET_OVER = "CMATCH_OO";
		static const std::string CHAMPIONSHIP_STATUS_CHANGE = "C_SCHANGE";

		static const std::string CHAMPIONSHIP_MATCH_STATUS = "CMATCH_STATUS";
		static const std::string CHAMPIONSHIP_MATCH_OPPONENT_WITHDRAW = "CMATCH_USER_OWD";
		static const std::string CHAMPIONSHIP_MATCH_WITHDRAW = "CMATCH_USER_WD";
		static const std::string CHAMPIONSHIP_MATCH_WAIT = "CMATCH_WAIT";

		static const std::string CHAMPIONSHIP_MATCH_PENDING_RESPONSE = "CMATCH_P_RES";
		static const std::string CHAMPIONSHIP_MATCH_READY = "CMATCH_USER_RDY";
		
		static const std::string CHAMPIONSHIP_MATCH_START = "CMATCH_START";
		static const std::string CHAMPIONSHIP_MATCH_NOT_FOUND = "CMATCH_NFOUND";

		static const std::string CHAMPIONSHIP_UNPLAYED_MATCH_WON = "CMATCH_UPLAYED_W";
		static const std::string CHAMPIONSHIP_UNPLAYED_MATCH_LOST = "CMATCH_UPLAYED_L";

		//End of match rapport------------
		static const std::string END_OF_MATCH_RAPPORT = "MR";
		static const std::string WON_MATCH = "MR_WON";
		static const std::string CHAMPIONSHIP_MATCH = "MR_CMATCH";
		static const std::string FAME_WON = "MR_FW";
		static const std::string FAME_LOST = "MR_FL";
		static const std::string MONEY_GAIN = "MR_$";
		static const std::string AP_WON = "MR_APWW";
		
		

	}
}

namespace gameconfig
{
	static const unsigned int SLEEP_TIME = 10;
	static const unsigned int TICK_TIME = 20;
	static const unsigned int TICKS_BEFORE_MATCH = 1;
	static const unsigned int TICKS_BEFORE_STADIUM = 1;
	static const unsigned int TICKS_BEFORE_CHAMPIONSHIP = 1;
	static const unsigned int TICKS_BEFORE_RESET = 1440;
	static const std::string UNNAMED_TEAM = "NO_NAME";
	static const int MAX_COACHES = 5;
	static const int MAX_PLAYERS = 100;
	static const int MIN_PLAYERS = 7;
	static const int STARTING_FUNDS = 100000;
	static const int STARTING_FAME = 1000;
	static const int STARTING_AC_POINTS=20;
	static const int SALE_FIRST_TURN_TIME = 30;
	static const int SALE_TURN_TIME = 30;
	static const float BID_RATIO = 0.05;
	static const int MAX_CHAMPIONSHIP_TURNS = 5; //32 teams
	static const int MAX_CHAMP_MATCH_OFFSET = 3600;
	static const float FAME_MULTIPLIER_INSTALLATION=0.25;
	static const float FAME_MULTIPLIER_B_GAME=0.23;//will determine the fame gained after a victory based on the other teams
	static const float FAME_MULTIPLIER_S_GAME=0.23;
	static const int FAME_EARN_DISCONNECT=30;
	static const int FAME_LOSE_DISCONNECT=30;
	static const int FUNDS_EARN_DISCONNECT=600;
	static const int FUNDS_EARN_GAME=900;
	static const int BIG_GOAL_DIFFERENCE = 100;
	static const float FUNDS_GAME_RATIO=0.05;
	static const float FUNDS_CHAMP_RATIO=0.12;
//	static const int MAX_CHAMP_MATCH_OFFSET = 61;
	static const int WINNER_LOSER_GAIN_RATIO = 4;
	static const int MONEY_GAIN_RATIO = 100;
	static const int AP_GAIN_RATIO = 1;
	static const int FAME_GAIN_RATIO = 10;
    static const int MAXAPTVAL = 100;
    static const int FAME_WIN = 100;
    static const int MATCH_WON_AP_GAIN = 10;
    static const int MATCH_LOST_AP_GAIN = 5;
    static const int TRIBUNE_SPECTATORS = 100;
    static const int FOOD_PRICE = 5;
    static const int TICKET_PRICE = 5;
    static const int GOODIES_PRICE = 5;

}

namespace memory
{
	// Directories
	static const std::string GLOBAL_DATA_DIR = "data/global/";
	static const std::string SKEL_DIR = "data/skel/";
	static const std::string USERS_DIR = "data/users/";
	static const std::string PLAYERS_DIR = "Players/";
	static const std::string INSTALLATIONS_DIR = "Installations/";
	static const std::string MARKET_PATH = "data/PlayerMarket/";
	static const std::string CHAMPIONSHIPS_DIR = "data/championships/";
	// Files
	static const std::string FILE_FORMAT = ".json";
	static const std::string USER_FILE = "user";
	static const std::string TEAM_INFOS_FILE = "teamInfos";
	static const std::string BROOM_FILE = "broomstick";
	static const std::string JERSEY_FILE = "jersey";
	static const std::string INSTS_SKEL_FILE = "installations";
	static const std::string TEAMNAMES_FILE = "teamNames";
	static const std::string USERNAMES_FILE = "userNames";
	// Data
	static const std::string FUNDS = "funds";
	static const std::string AC_POINTS = "activity_points";
	static const std::string TEAM_NAME = "teamname";
	static const std::string FAME = "fame";
	static const std::string SALE_FILE = "_sale";
	static const std::string INST_TYPE = "name";
	static const std::string LEVEL = "level";
	static const std::string BASE_VALUE = "baseValue";
	static const std::string REFUND_RATIO = "refundratio";
	static const std::string FANSHOP = "Fan Shop";
	static const std::string MEDIC_CENTER = "Medical Center";
	static const std::string FOOD_STAND = "Food Stand";
	static const std::string TRIBUNE = "Tribune";
	static const std::string ALL_TEAM_NAMES = "A_T_N";
	static const std::string ALL_USER_NAMES = "A_U_N";

	static const std::string ADMIN_DIR = "data/admins/";
	static const std::string ADMIN_FILE = "admin";
}

#endif
