#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <sstream>
#include "bonus.h"
#include "playerlogger.h"
#include "file.h"

template <typename T>
class Enemy;
class Fire;
class Ice;
class Earth;


class Player: public QGraphicsRectItem
{
public:
    static Player *GetInstance();
    enum directions {idle = 0, left = 1, right = 2, up = 3, down = 4};
    enum playerType {elec = 0, earth = 1, ice = 2};

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void operator+ (IBonus* bns);
    void operator+ (Enemy<Fire>* enemy);
    void operator+ (Enemy<Ice>* enemy);
    void operator+ (Enemy<Earth>* enemy);

    int getX() const;
    int getY() const;
    int getScale() const;
    directions getDirection() const;
    int getKeys() const;
    playerType getType() const;
    int getHP() const;

    void setDirection(directions direction);
    void setX(int n);
    void setY(int n);
    void setScale(int n);
    void setKeys(int n);
    void setType(playerType type);
    void setType(int type);
    void setHP(int n);

    bool isDead() const;

    std::string savePlayer();
    void loadPlayer(std::string playerstr);
    PlayerLogger *playerlog;
private:
    static Player *instance_p;
    Player();
    ~Player();
    Player(Player const& f) = delete;
    Player(Player&& f) = delete;

    directions direction;
    playerType plType;

    QPixmap *idle_elec, *idle_earth, *idle_ice;
    int x, y, scale, animatestate, keycollected, hp;

    QPixmap* getPixmapType();
    File* file;
};


#endif // PLAYER_H
