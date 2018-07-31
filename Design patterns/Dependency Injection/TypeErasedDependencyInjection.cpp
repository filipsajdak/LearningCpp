#include <string>
#include "FileLogger.h"
#include "User.h"
#include "UsersFileDataBase.h"

/*
    Logger implements type-erasure technique which can make your non-related classes working as they inherit from one base class; 

    There is possibility of writing such wrapper that will work for classes with different methods to log messages - it will use SFINAE to achive that.
*/
class Logger {
    public:
        template <typename T>
        Logger(T* logger) 
        : logger(logger)
        , log_fun([](void* l, const std::string& msg){
            reinterpret_cast<T*>(l).log(msg);
        }) 
        {
            // empty
        }

        void log(const std::string& msg) {
            log_fun(logger, msg);
        }

    private:
        void* logger;
        void(*log_fun)(void*,const std::string&);
};

class UsersDataBaseTypeErased {
    public:
        template <typename T>
        UsersDataBaseTypeErased(T* users) 
        : users(users)
        , findUser_fun([](void* u, const std::string& login){
            reinterpret_cast<T*>(u).findUser(login);
        }) 
        {
            // empty
        }

        void log(const std::string& login) {
            findUser_fun(users, login);
        }

    private:
        void* users;
        void(*findUser_fun)(void*,const std::string&);
};

class Authorization
{
  public:
    Authorization(UsersDataBaseTypeErased udb, Logger logger) : users(std::move(udb), logger(std::move(logger)) {}

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
    UsersDataBaseTypeErased users;
    Logger logger;
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
    Authorization auth(&users, &logger);

    EXPECT_CALL(users, findUser(user.login)).WillOnce(Return(&user));

    ASSERT_THAT(auth.authorize(user.login, user.password), true);
}

TEST(AuthorizationTests, type_erasured_incorrect_login)
{
    User user{"login", "password"};
    UsersDataBaseMock users;
    LoggerMock logger;
    Authorization auth(&users, &logger);

    InSeguence _;
    EXPECT_CALL(users, findUser(user.login)).WillOnce(Return(&user));
    EXPECT_CALL(logger, log("User " + user.login + " failed to log in"));

    ASSERT_THAT(auth.authorize(user.login, "bad password"), false);
}