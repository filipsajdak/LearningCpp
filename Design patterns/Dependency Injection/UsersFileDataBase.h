#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include "UsersDataBase.h"
#include "User.h"

class UsersFileDataBase : public UsersDataBase
{
  public:
    UsersFileDataBase();

    const User *findUser(std::string login) override;

  private:
    std::unordered_map<std::string, User> users;
};
