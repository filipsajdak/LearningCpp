#pragma once
#include <string>

struct User;

class  UsersDataBase
{
  public:
    virtual ~UsersDataBase() = default;

    virtual const User* findUser(std::string login) = 0;
};