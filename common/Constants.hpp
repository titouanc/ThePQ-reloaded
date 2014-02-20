#ifndef __CONSTANTS_HPP
#define __CONSTANTS_HPP

namespace net
{
	// NETWORK MESSAGES --------------------------------------------------------
	namespace MSG
	{
		// TYPE ----------------------------------------------
		static const std::string DISCONNECT = "D";
		static const std::string LOGIN = "Q_L";
		static const std::string REGISTER = "Q_R";
		static const std::string USER_EXISTS = "U_E";
		static const std::string STATUS = "Q_S";
		static const std::string INSTALLATIONS_LIST = "DQ_I_L";
		static const std::string INSTALLATION_UPGRADE = "AQ_I_U";
		static const std::string INSTALLATION_DOWNGRADE = "AQ_I_D";
		static const std::string CONNECTED_USERS_LIST = "DQ_CU_L";
		
		// DATA ----------------------------------------------
		// connection + register
		static const std::string USERNAME = "D_U";
		static const std::string PASSWORD = "D_P";
		static const std::string FUNDS = "D_F";
		static const std::string USER_LOGIN = "U_L";
		static const std::string PASSWORD_ERROR = "U_P";
		static const std::string USER_NOT_FOUND = "U_NF";
		static const std::string USER_REGISTERED = "U_R";
		// data queries
	}
}

#endif
