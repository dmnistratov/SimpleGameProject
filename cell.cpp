#include "cell.h"

Cell::Cell()
{
    cell = new QPixmap(":/images/land_1.png");
    empty_cell = new QPixmap(":/images/land_empty.png");
    border_cell = new QPixmap(":/images/border.png");
    start_cell = new QPixmap(":/images/start.png");
    exit_cell = new QPixmap(":/images/exit.png");
    x = 0, y = 0, scale = 0, type = filled;
}

Cell::~Cell()
{
    delete cell;
    delete empty_cell;
    delete border_cell;
    delete start_cell;
    delete exit_cell;
}


int Cell::getX() const
{
    return x;
}

int Cell::getY() const
{
    return y;
}

int Cell::getType() const
{
    return type;
}

void Cell::setX(int n)
{
    x = n;
}

void Cell::setY(int n)
{
    y = n;
}

void Cell::setType(Cell::celltype n)
{
    type = n;
}

void Cell::setType(int n)
{
    switch (n) {
    case -2:
        type = celltype::border;
        break;
    case -1:
        type = celltype::filled;
        break;
    case 0:
        type = celltype::empty;
        break;
    case 1:
        type = celltype::entry;
        break;
    case 2:
        type = celltype::exit;
        break;
    default:
        throw std::invalid_argument("Save file corrupted");
    }
}

void Cell::setScale(int n)
{
    scale = n;
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    switch (type) {
    case border:
        painter->drawPixmap(scale*x, scale*y, border_cell->scaled(QSize(scale,scale), Qt::KeepAspectRatio), 0, 0, scale, scale);
        break;
    case filled:
        painter->drawPixmap(scale*x, scale*y, cell->scaled(QSize(scale,scale), Qt::KeepAspectRatio), 0, 0, scale, scale);
        break;
    case empty:
        painter->drawPixmap(scale*x, scale*y, empty_cell->scaled(QSize(scale,scale), Qt::KeepAspectRatio), 0, 0, scale, scale);
        break;
    case entry:
        painter->drawPixmap(scale*x, scale*y, start_cell->scaled(QSize(scale,scale), Qt::KeepAspectRatio), 0, 0, scale, scale);
        break;
    case exit:
        painter->drawPixmap(scale*x, scale*y, exit_cell->scaled(QSize(scale,scale), Qt::KeepAspectRatio), 0, 0, scale, scale);
        break;
    }
}
