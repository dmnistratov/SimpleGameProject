#include "playerlogger.h"

PlayerLogger::PlayerLogger(File *file)
{
    log = file;
}

void PlayerLogger::operator<<(std::string str)
{
    std::string players = "[PLAYER] ";
    players.append(str);
    log->write(players);
}
