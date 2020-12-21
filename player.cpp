#include "player.h"
#include "bonus.h"
#include "enemy.h"
#include <iostream>

Player* Player::instance_p = nullptr;
Player *Player::GetInstance()
{
    if (!instance_p)
        instance_p = new Player();
    return instance_p;
}

Player::Player()
{
    idle_elec = new QPixmap(":/images/player_elec.png");
    idle_earth = new QPixmap(":/images/player_earth.png");
    idle_ice = new QPixmap(":/images/player_ice.png");
    direction = idle;
    plType = elec;
    x = 0, y = 0, animatestate = 0, scale = 0, keycollected = 0, hp = 2;
    file = new File("logfile.txt");
    playerlog = new PlayerLogger(file);
        *playerlog << "Player created";
}

Player::~Player()
{
    *playerlog << "deconstructor";
    delete idle_elec;
    delete idle_earth;
    delete idle_ice;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(scale*x, scale*y, (this->getPixmapType())->scaled(QSize(scale*1.5,scale*1.5), Qt::KeepAspectRatio), 0, 0, scale*1.5, scale*1.5);

    // #TODO Sprites for different movements
   /* switch (direction) {
    case idle:
        painter->drawPixmap(scale*x, scale*y, (this->getPixmapType())->scaled(QSize(scale*1.5,scale*1.5), Qt::KeepAspectRatio), 0, 0, scale*1.5, scale*1.5);
        break;
    case left:
        painter->drawPixmap(scale*x, scale*y, (this->getPixmapType())->scaled(QSize(scale*1.5,scale*1.5), Qt::KeepAspectRatio), 0, 0, scale*1.5, scale*1.5);
        break;
    case right:
        painter->drawPixmap(scale*x, scale*y, (this->getPixmapType())->scaled(QSize(scale*1.5,scale*1.5), Qt::KeepAspectRatio), 0, 0, scale*1.5, scale*1.5);
        break;
    case up:
        painter->drawPixmap(scale*x, scale*y, (this->getPixmapType())->scaled(QSize(scale*1.5,scale*1.5), Qt::KeepAspectRatio), 0, 0, scale*1.5, scale*1.5);
        break;
    case down:
        painter->drawPixmap(scale*x, scale*y, (this->getPixmapType())->scaled(QSize(scale*1.5,scale*1.5), Qt::KeepAspectRatio), 0, 0, scale*1.5, scale*1.5);
        break;
    }*/
}

void Player::operator+(IBonus *bns)
{
        *playerlog << "bonus picked";
    bns->pickedb();
}

void Player::operator+(Enemy<Fire> *enemy)
{
    switch (enemy->getState()) {
    case Enemy<Fire>::KILL:
        hp = 0;
        break;
    case Enemy<Fire>::DMG:
        hp -= 1;
        *this + new tp_to_spawn();
        break;
    case Enemy<Fire>::IDLE:
        break;
    }
}

void Player::operator+(Enemy<Ice> *enemy)
{
    switch (enemy->getState()) {
    case Enemy<Ice>::KILL:
        hp = 0;
        break;
    case Enemy<Ice>::DMG:
        hp -= 1;
        *this + new tp_to_spawn();
        break;
    case Enemy<Ice>::IDLE:
        break;
    }
}

void Player::operator+(Enemy<Earth> *enemy)
{
    switch (enemy->getState()) {
    case Enemy<Earth>::KILL:
        hp = 0;
        break;
    case Enemy<Earth>::DMG:
        hp -= 1;
        *this + new tp_to_spawn();
        break;
    case Enemy<Earth>::IDLE:
        break;
    }
}

int Player::getX() const
{
    return x;
}

int Player::getY() const
{
    return y;
}

int Player::getScale() const
{
    return scale;
}

Player::directions Player::getDirection() const
{
    return direction;
}

int Player::getKeys() const
{
    std::cout << keycollected << std::endl;
    return keycollected;
}

Player::playerType Player::getType() const
{
    return plType;
}

int Player::getHP() const
{
    return hp;
}

void Player::setDirection(Player::directions dir)
{
        *playerlog << "new direction";
    direction = dir;
}

void Player::setX(int n)
{
        *playerlog << "x changed";
    x = n;
}

void Player::setY(int n)
{
        *playerlog << "y changed";
    y = n;
}

void Player::setScale(int n)
{
        *playerlog << "new scale";
    scale = n;
}

void Player::setKeys(int n)
{
        *playerlog << "key added";
    keycollected = n;
}

void Player::setType(Player::playerType type)
{
        *playerlog << "new type";
    plType = type;
}

void Player::setType(int type)
{
    switch (type) {
    case 0:
        this->setType(elec);
        break;
    case 1:
        this->setType(earth);
        break;
    case 2:
        this->setType(ice);
        break;
    default:
        throw "Save file corrupted";
    }
}

void Player::setHP(int n)
{
    hp = n;
}

bool Player::isDead() const
{
    if (hp < 1)
        return true;
    return false;
}

std::string Player::savePlayer()
{
    std::string tempstr;
    tempstr.append(std::to_string(x));
    tempstr.append(" ");
    tempstr.append(std::to_string(y));
    tempstr.append(" ");
    tempstr.append(std::to_string(scale));
    tempstr.append(" ");
    tempstr.append(std::to_string(keycollected));
    tempstr.append(" ");
    tempstr.append(std::to_string(hp));
    tempstr.append(" ");
    tempstr.append(std::to_string(plType));
    tempstr.append(" ");
    return tempstr;
}

void Player::loadPlayer(std::string playerstr)
{
    std::stringstream ss;
    ss << playerstr;
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
    ss >> val;
    keycollected = val;
    ss >> val;
    hp = val;
    ss >> val;
    setType(val);
}

QPixmap *Player::getPixmapType()
{
    switch (plType) {
    case earth:
        return idle_earth;
    case ice:
        return idle_ice;
    case elec:
        return idle_elec;
    }
    return idle_elec;
}

