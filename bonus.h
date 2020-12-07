#ifndef BONUS_H
#define BONUS_H

#include <QGraphicsRectItem>
#include <QPixmap>

enum BonusType{
    key_b, rainbow_b, tp_b
};

class IBonus: public QGraphicsRectItem
{
public:
    IBonus();
    virtual ~IBonus();
    virtual void pickedb();
    virtual void place(int x_n, int y_n);
    virtual void setScale(int n);

    virtual int getX() const;
    virtual int getY() const;
};

class key: public IBonus
{
public:
    key();
    ~key();

     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

     void pickedb();
     void place(int x_n, int y_n);
     void setScale(int n);

     int getX() const;
     int getY() const;

private:
     QPixmap* keyI;
     int x, y, scale;
     bool collected;
};

class tp_to_spawn: public IBonus
{
public:
     tp_to_spawn();
     ~tp_to_spawn();

     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

     void pickedb();
     void place(int x_n, int y_n);
     void setScale(int n);

     int getX() const;
     int getY() const;

private:
     QPixmap* tpI;
     int x, y, sx, sy, scale;

     void setSpawnLoc();
};

class rainbow: public IBonus
{
public:
    rainbow();
    ~rainbow();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void pickedb();
    void place(int x_n, int y_n);
    void setScale(int n);

    int getX() const;
    int getY() const;

private:
     QPixmap* rainbowI;
     int x, y, scale;
};

class IBonusFactory
{
public:
    IBonusFactory();;
    ~IBonusFactory();;

    virtual IBonus* CreateBonus(BonusType type);
};

class BonusFactory: public IBonusFactory
{
public:
    BonusFactory();
    ~BonusFactory();

    IBonus *CreateBonus(BonusType type);
};
#endif // BONUS_H
