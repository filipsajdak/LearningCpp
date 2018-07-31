#include <string>
#include "FileLogger.h"
#include "User.h"
#include "UsersFileDataBase.h"

class Authorization
{
  public:
    Authorization(std::function<const User*(const std::string&)> findUser, std::function<void(const std::string&)> log) : findUser(std::move(findUser), log(std::move(log)) {}

    bool authorize(std::string login, std::string password)
    {
        auto user = findUser(login);

        if (user && user->password == password)
        {
            return true;
        }

        log("User " + login + " failed to log in");
        return false;
    }

  private:
    std::function<const User*(const std::string&)> findUser;
    std::function<void(const std::string&)> log;
};

#include <gmock/gmock.h>

using namespace ::testing;

class UsersDataBaseMock {
    public:
        MOCK_METHOD1(findUser, const User*(std::string));
};

class LoggerMock { // Please notice that it is not inheriting any interface or baseclass
    public:
        MOCK_METHOD1(log, void(const std::string&));
};

TEST(AuthorizationTests, type_erasured_correct_login)
{
    User user{"login", "password"};
    UsersDataBaseMock users;
    LoggerMock logger;
    Authorization auth([&users](const std::string& l){return users.findUser(l);}, [&logger](const std::string& m){logger.log(m);});

    EXPECT_CALL(users, findUser(user.login)).WillOnce(Return(&user));

    ASSERT_THAT(auth.authorize(user.login, user.password), true);
}

TEST(AuthorizationTests, type_erasured_incorrect_login)
{
    User user{"login", "password"};
    UsersDataBaseMock users;
    LoggerMock logger;
    Authorization auth([&users](const std::string& l){return users.findUser(l);}, [&logger](const std::string& m){logger.log(m);});

    InSeguence _;
    EXPECT_CALL(users, findUser(user.login)).WillOnce(Return(&user));
    EXPECT_CALL(logger, log("User " + user.login + " failed to log in"));

    ASSERT_THAT(auth.authorize(user.login, "bad password"), false);
}