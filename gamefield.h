#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "cell.h"
#include "gamefielditerator.h"
#include <iostream>
#include "player.h"
#include "bonus.h"
#include "fieldlogger.h"
#include "file.h"

static const size_t MAP_RESOLUTION = 800;

class GameField
{
public:
    static GameField* GetInstance();

    void setSize(size_t n, size_t h);
    Cell*** createField();
    void clearField();

    class GameFieldIterator begin();
    class GameFieldIterator end();
    void PrintXY();
    void generateMap();
    bool isCollide(int x, int y);
    bool isExit(int x, int y);
    IBonus* pickup(int x, int y);

    IBonus *keyb, *tpb, *rainb;
    FieldLogger *fieldlog;

private:
    static GameField *instance_p;

    GameField();
    ~GameField();
    GameField(GameField const& f) = delete;
    GameField(GameField&& f) = delete;
    GameField& operator = (GameField const& f) = delete;
    GameField& operator = (GameField && f) = delete;

    Cell*** field;
    size_t m_height, m_width;
    IBonusFactory *bonuses;
    File *file;

};

#endif // GAMEFIELD_H
