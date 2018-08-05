#include <string>
#include "FileLogger.h"
#include "User.h"
#include "UsersFileDataBase.h"

namespace fixed_deps {
	class Authorization
	{
	public:
		bool authorize(std::string login, std::string password)
		{
			auto user = users.findUser(login);

			if (user && user->password == password)
			{
				return true;
			}

			logger.log("User " + login + " failed to log in");
			return false;
		}

	private:
		UsersFileDataBase users;
		FileLogger logger;
	};
}
/*
    UsersFileDataBase can be mocked in link time but FileLogger is header-only lib (it will be probably inlined by the compiler);

    Such design is hard to mocked as all dependencies are fixed.
*/
