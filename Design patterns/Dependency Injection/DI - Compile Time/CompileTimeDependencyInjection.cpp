#include <string>
#include <gmock/gmock.h>

#include "FileLogger.h"
#include "User.h"
#include "UsersFileDataBase.h"

namespace {

template <typename UsersDataBase, typename PolymorphicFileLogger>
class Authorization
{
  public:
    Authorization(UsersDataBase &udb, PolymorphicFileLogger &logger) : users(udb), logger(logger) {}

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
    UsersDataBase &users;
    PolymorphicFileLogger &logger;
};

using namespace ::testing;

class UsersDataBaseMock
{ // no inheritance, no baseclass
  public:
    MOCK_METHOD1(findUser, const User *(std::string));
};

class LoggerMock
{ // no inheritance, no baseclass
  public:
    MOCK_METHOD1(log, void(const std::string &));
};

TEST(AuthorizationTests_compile_time, correct_login)
{
    User user{"login", "password"};
    UsersDataBaseMock users;
    LoggerMock logger;
    Authorization<UsersDataBaseMock, LoggerMock> auth(users, logger);

    EXPECT_CALL(users, findUser(user.login)).WillOnce(Return(&user));

    ASSERT_THAT(auth.authorize(user.login, user.password), true);
}

TEST(AuthorizationTests_compile_time, incorrect_login)
{
    User user{"login", "password"};
    UsersDataBaseMock users;
    LoggerMock logger;
    Authorization<UsersDataBaseMock, LoggerMock> auth(users, logger);

    InSequence _;
    EXPECT_CALL(users, findUser(user.login)).WillOnce(Return(&user));
    EXPECT_CALL(logger, log("User " + user.login + " failed to log in"));

    ASSERT_THAT(auth.authorize(user.login, "bad password"), false);
}
}