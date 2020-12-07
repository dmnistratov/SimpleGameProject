#ifndef PLAYERLOGGER_H
#define PLAYERLOGGER_H

#include "file.h"

class PlayerLogger
{
public:
    PlayerLogger(File *file);

    void operator<<(std::string str);

private:
    File* log;
};

#endif // PLAYERLOGGER_H
