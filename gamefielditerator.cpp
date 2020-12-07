#include "gamefielditerator.h"

Cell *&GameFieldIterator::operator*()
{
    return *current;
}

GameFieldIterator &GameFieldIterator::operator++(int)
{
    this->operator++();
    return *this;
}

bool GameFieldIterator::operator==(const GameFieldIterator &rhs)
{
    return (this->current == rhs.current);
}

GameFieldIterator &GameFieldIterator::operator++()
{
    current_col++;
    if (current_col == size_width && current_row == size_height-1)
    {
        current = (*(gamefield+current_row)+current_col);
        return *this;
    }
    if (current_col == size_width && current_row != size_height){
        current_row++;
        current_col = 0;
    }
    if (current_row == size_height){
        current_row = 0;
        current_col = 0;
    }
    current = (*(gamefield+current_row)+current_col);
    return *this;
}

bool GameFieldIterator::operator!=(const GameFieldIterator &rhs)
{
    return (this->current != rhs.current);
}
