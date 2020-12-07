#ifndef CELL_H
#define CELL_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <iostream>

//class of a single cell
class Cell: public QGraphicsRectItem
{
public:
    Cell();
    ~Cell();

    enum celltype {border = -2, filled = -1, empty = 0, entry = 1, exit = 2};

    int getX() const;
    int getY() const;
    int getType() const;

    void setX(int n);
    void setY(int n);
    void setType(celltype n);
    void setScale(int n);

private:
    int x,y, scale;
    celltype type;
    QPixmap *cell, *empty_cell, *border_cell, *start_cell, *exit_cell;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // CELL_H
