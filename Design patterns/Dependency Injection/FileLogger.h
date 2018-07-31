#pragma once
#include <fstream>

class FileLogger
{
  public:
    FileLogger() : out("log.txt") {}
    void log(const std::string& msg)
    {
        out << msg << std::endl;
    }

  private:
    std::ofstream out;
};