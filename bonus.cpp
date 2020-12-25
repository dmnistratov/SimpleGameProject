#include "bonus.h"
#include "gamefield.h"
#include "player.h"

IBonus::IBonus() {}

IBonus::~IBonus() {}

void IBonus::pickedb() {}

void IBonus::place(int x_n, int y_n)
{
    Q_UNUSED(x_n);
    Q_UNUSED(y_n);
}

void IBonus::setScale(int n)
{
    Q_UNUSED(n);
}

int IBonus::getX() const
{
    return 0;
}

int IBonus::getY() const
{
    return 0;
}

void IBonus::setStat(int n)
{
    Q_UNUSED(n);
}

std::string IBonus::saveBonus() const
{
    return NULL;
}

key::key()
{
    keyI = new QPixmap(":/images/key.png");
    x = 0, y = 0, scale = 0;
    collected = false;
}

key::~key()
{
    delete keyI;
}

void key::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!collected)
        painter->drawPixmap(scale*x, scale*y, keyI->scaled(QSize(scale, scale), Qt::KeepAspectRatio), 0, 0, scale, scale);
}

void key::pickedb()
{
    Player* player = Player::GetInstance();
    collected = true;
    std::cout << "picked" << std::endl;
    player->setKeys(1);
}

void key::place(int x_n, int y_n)
{
    x = x_n;
    y = y_n;
}

void key::setScale(int n)
{
    scale = n;
}

int key::getX() const
{
    return x;
}

int key::getY() const
{
    return y;
}

void key::setStat(int n)
{
    collected = n;
}

std::string key::saveBonus() const
{
    std::string temp;
    temp.append(std::to_string(x));
    temp.append(" ");
    temp.append(std::to_string(y));
    temp.append(" ");
    temp.append(std::to_string(scale));
    temp.append(" ");
    temp.append(std::to_string(collected));
    temp.append(" \n");
    return temp;
}

tp_to_spawn::tp_to_spawn()
{
    tpI = new QPixmap(":/images/teleport.png");
    x = 0, y = 0, sx = 0, sy = 0, scale = 0;
}

tp_to_spawn::~tp_to_spawn()
{
    delete tpI;
}

void tp_to_spawn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(scale*x, scale*y, tpI->scaled(QSize(scale, scale), Qt::KeepAspectRatio), 0, 0, scale, scale);
}

void tp_to_spawn::pickedb()
{
    Player* player = Player::GetInstance();
    setSpawnLoc();
    player->setX(sx);
    player->setY(sy);
}

void tp_to_spawn::place(int x_n, int y_n)
{
    x = x_n;
    y = y_n;
}

void tp_to_spawn::setScale(int n)
{
    scale = n;
}

int tp_to_spawn::getX() const
{
    return x;
}

int tp_to_spawn::getY() const
{
    return y;
}

void tp_to_spawn::setStat(int n)
{
        Q_UNUSED(n);
}

std::string tp_to_spawn::saveBonus() const
{
    std::string temp;
    temp.append(std::to_string(x));
    temp.append(" ");
    temp.append(std::to_string(y));
    temp.append(" ");
    temp.append(std::to_string(scale));
    temp.append(" \n");
    return temp;
}

void tp_to_spawn::setSpawnLoc()
{
    GameField* field = GameField::GetInstance();
    GameFieldIterator st_test = field->begin();
    GameFieldIterator end = field->end();
    for (; st_test != end; ++st_test){
         if ((*st_test)->getType() == Cell::entry)
         {
             sx = (*st_test)->getX();
             sy = (*st_test)->getY();
             return;
         }
     }
}

rainbow::rainbow()
{
    rainbowI = new QPixmap(":/images/Tavern.png");
    x = 0, y = 0, scale = 0;
}

rainbow::~rainbow()
{
    delete rainbowI;
}

void rainbow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(scale*x, scale*y, rainbowI->scaled(QSize(scale, scale), Qt::KeepAspectRatio), 0, 0, scale, scale);
}

void rainbow::pickedb()
{
    Player* player = Player::GetInstance();
    int t = rand() % 3;
    switch (t) {
    case 0:
        player->setType(Player::earth);
        break;
    case 1:
        player->setType(Player::elec);
        break;
    case 2:
        player->setType(Player::ice);
        break;
    }
}

void rainbow::place(int x_n, int y_n)
{
    x = x_n;
    y = y_n;
}

void rainbow::setScale(int n)
{
    scale = n;
}

int rainbow::getX() const
{
    return x;
}

int rainbow::getY() const
{
    return y;
}

void rainbow::setStat(int n)
{
        Q_UNUSED(n);
}

std::string rainbow::saveBonus() const
{
    std::string temp;
    temp.append(std::to_string(x));
    temp.append(" ");
    temp.append(std::to_string(y));
    temp.append(" ");
    temp.append(std::to_string(scale));
    temp.append(" ");
    return temp;
}

IBonusFactory::IBonusFactory() {}

IBonusFactory::~IBonusFactory() {}

IBonus *IBonusFactory::CreateBonus(BonusType type)
{
    Q_UNUSED(type);
    return nullptr;
}

BonusFactory::BonusFactory() {}

BonusFactory::~BonusFactory() {}

IBonus *BonusFactory::CreateBonus(BonusType type)
{
    switch (type) {
    case key_b:
        return new key();
        break;
    case rainbow_b:
        return new rainbow();
        break;
    case tp_b:
        return new tp_to_spawn();
        break;
    default:
        throw std::invalid_argument("unknown bonustype");
    }
    return nullptr;
}
