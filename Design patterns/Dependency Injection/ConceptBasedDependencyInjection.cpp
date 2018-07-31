#include <string>
#include <memory>

#include "FileLogger.h"
#include "User.h"
#include "UsersFileDataBase.h"

class Logger {
    public:
        template <typename T>
        Logger(T* l) : logger_ptr(new logger<T>(l) 
        {
            // empty
        }

        void log(const std::string& msg) {
            logger_ptr->log(msg);
        }

    private:
        struct ilogger {
            virtual ~ilogger() = default;
            virtual void log(const std::string& msg) = 0;
        };

        template <typename T>
        struct logger : ilogger {
            logger(T* l) : l(l) {}
            
            void log(const std::string& msg) {
                l->log(msg);
            }

            private:
            T* l;
        }

        std::unique_ptr<ilogger> logger_ptr;
};

class  UsersDB {
    public:
        template <typename T>
        UsersDB(T* l) : db_ptr(new db<T>(l) 
        {
            // empty
        }

        const User* findUser(const std::string& login) {
            return db_ptr->findUser(login);
        }

    private:
        struct idb {
            virtual ~idb() = default;
            virtual const User* findUser(const std::string& login) = 0;
        };

        template <typename T>
        struct db : idb {
            db(T* l) : l(l) {}
            
            const USer* findUser(const std::string& login) {
                return l->findUser(login);
            }

            private:
            T* l;
        }

        std::unique_ptr<idb> db_ptr;
};

class Authorization
{
  public:
    Authorization(UsersDB udb, Logger logger) : users(std::move(udb), logger(std::move(logger)) {}

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
    UsersDB users;
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