#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <QGraphicsRectItem>
#include "player.h"
#include <QPainter>
class Earth;
class Ice;
class Fire;

template <typename T>
class Enemy: public QGraphicsRectItem
{
public:

    enum action {DMG, KILL, IDLE};

    Enemy()
    {
        API = new T();
        x = 0, y = 0, scale = 0;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->drawPixmap(scale*x, scale*y, API->getImage()->scaled(QSize(scale*1.3,scale*1.3), Qt::KeepAspectRatio), 0, 0, scale*1.3, scale*1.3);
    }

    void setX(int n)
    {
        x = n;
    }
    void setY(int n)
    {
        y = n;
    }
    void setScale(int n)
    {
        scale = n;
    }

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    void operator+ (Player* player)
    {
        *API + player;
    }

    std::string saveEnemy()
    {
        std::string tempstr;
        tempstr.append(std::to_string(x));
        tempstr.append(" ");
        tempstr.append(std::to_string(y));
        tempstr.append(" ");
        tempstr.append(std::to_string(scale));
        tempstr.append(" ");
        return tempstr;
    }

    void loadEnemy(std::string enemystr)
    {
        std::stringstream ss;
        ss << enemystr;
        int val;
        ss >> val;
        if (val > 10) //#Fix
            throw "X error";
        x = val;
        ss >> val;
        if (val > 10) //#Fix
            throw "X error";
        y = val;
        ss >> val;
        scale = val;
    }

    action getState() const
    {
        return API->setState();
    }
private:
    T* API;
    int x,y, scale;
};

class Ice
{
public:
    Ice()
    {
        idle_ice = new QPixmap(":/images/enemy_ice.png");
    }

    ~Ice()
    {
        delete idle_ice;
    }
    void operator+ (Player* player)
    {
        if (player->getType() == Player::elec)
            state = Enemy<Ice>::KILL;
        else if (player->getType() == Player::earth)
        {
            state = Enemy<Ice>::DMG;
        }
    }

    QPixmap* getImage() const
    {
        return idle_ice;
    }

    Enemy<Ice>::action setState()
    {
        return state;
    }
private:
    QPixmap* idle_ice;
    Enemy<Ice>::action state = Enemy<Ice>::IDLE;
};

class Fire
{
public:
    Fire()
    {
        idle_fire = new QPixmap(":/images/enemy_fire.png");
    }

    ~Fire()
    {
        delete idle_fire;
    }
    void operator+ (Player* player)
    {
        if (player->getType() == Player::earth)
            state = Enemy<Fire>::KILL;
        else if (player->getType() == Player::ice)
        {
            state = Enemy<Fire>::DMG;
        }
    }

    QPixmap* getImage() const
    {
        return idle_fire;
    }

    Enemy<Fire>::action setState()
    {
        return state;
    }
private:
    QPixmap* idle_fire;
    Enemy<Fire>::action state = Enemy<Fire>::IDLE;
};

class Earth
{
public:
    Earth()
    {
        idle_earth = new QPixmap(":/images/enemy_earth.png");
    }

    ~Earth()
    {
        delete idle_earth;
    }
    void operator+ (Player* player)
    {
        if (player->getType() == Player::ice)
            state = Enemy<Earth>::KILL;
        else if (player->getType() == Player::elec)
        {
            state = Enemy<Earth>::DMG;
        }
    }

    QPixmap* getImage() const
    {
        return idle_earth;
    }

    Enemy<Earth>::action setState()
    {
        return state;
    }
private:
    QPixmap* idle_earth;
    Enemy<Earth>::action state = Enemy<Earth>::IDLE;
};
#endif // ENEMY_H
