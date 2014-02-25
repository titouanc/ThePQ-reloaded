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

		static const std::string ADD_PLAYER_ON_MARKET_QUERY = "Q_A_P_OM";	
		static const std::string DELETE_PLAYER_OF_MARKET_QUERY = "Q_D_P_OM";
		static const std::string BID_ON_PLAYER_QUERY = "Q_B_OP"; 			

		// DATA ----------------------------------------------
		// connection + register
		static const std::string USERNAME = "D_U";
		static const std::string PASSWORD = "D_P";
		static const std::string FUNDS = "D_F";
		static const std::string USER_LOGIN = "U_L";
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
		// data queries
		static const std::string INSTALLATIONS_LIST = "DQ_I_L";
		static const std::string CONNECTED_USERS_LIST = "DQ_CU_L";
		static const std::string PLAYERS_ON_MARKET_LIST= "DQ_P_OM_L";
		static const std::string PLAYERS_LIST = "DQ_P_L";

		// friendly game
		static const std::string FRIENDLY_GAME_USERNAME = "F_G_U";
		static const std::string FRIENDLY_GAME_INVITATION = "F_G_I";
		static const std::string FRIENDLY_GAME_INVITATION_RESPONSE = "F_G_I_R";
		static const std::string FRIENDLY_GAME_INVITATION_ACCEPT = "F_G_I_A";
		static const std::string FRIENDLY_GAME_INVITATION_DENY = "F_G_I_D";

		// MATCH ----------------------------------------------------------------
		static const std::string MATCH_START   = "MSTART";
		static const std::string MATCH_END     = "MEND";
		static const std::string MATCH_SQUADS  = "MSQUADS";
		static const std::string MATCH_BALLS   = "MBALLS";
		static const std::string MATCH_PROMPT  = "M?";
		static const std::string MATCH_TIMEOUT = "MTOUT";
		static const std::string MATCH_STROKE  = "MSTROKE";
		static const std::string MATCH_ERROR   = "M!!!";
		static const std::string MATCH_ACK     = "MACK";
		static const std::string MATCH_DELTA   = "MDELTA";
		static const std::string MATCH_SCORES  = "MSCORES";
		static const std::string MATCH_STATUS  = "MSTATUS";
	}
}

#endif
