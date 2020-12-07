#ifndef GAMEFIELDITERATOR_H
#define GAMEFIELDITERATOR_H

#include "gamefield.h"

class GameFieldIterator: public std::iterator<std::input_iterator_tag, Cell*>
{
public:
    GameFieldIterator(Cell** init, size_t size_w, size_t size_h, Cell*** field): gamefield(field), current(init), size_width(size_w), size_height(size_h) {};

    Cell*& operator*();
    GameFieldIterator& operator++();
    GameFieldIterator& operator++(int);
    bool operator==(const GameFieldIterator& rhs);
    bool operator!=(const GameFieldIterator& rhs);
private:
    Cell*** gamefield;
    Cell** current;
    size_t size_width, size_height, current_col = 0, current_row = 0;
};

#endif // GAMEFIELDITERATOR_H
