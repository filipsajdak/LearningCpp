#include "UsersFileDataBase.h"
#include <fstream>
#include <sstream>
#include <string>

UsersFileDataBase::UsersFileDataBase()
{
    std::ifstream data("users.dat");
    for (std::string line; std::getline(data, line);)
    {
        std::istringstream row(line);
        std::string user, password;
        std::getline(row, user, ';');
        std::getline(row, password);
        users[user] = {user, password};
    }
}

const User *UsersFileDataBase::findUser(std::string login)
{
    auto it = users.find(login);
    return it == users.end() ? nullptr : &it->second;
}
