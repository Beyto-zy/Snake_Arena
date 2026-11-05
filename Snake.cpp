#include "Snake.h"

Snake::Snake(char _id, Direction _direction)
    : id(_id), direction(_direction), score(0), alive(true), next(nullptr) {}

bool Snake::turn(Direction newDir)
{
    // TODO: The second segment is the "neck". If moving one cell in newDir would put
    // the head onto the neck, return false and change nothing. Otherwise set the
    // direction and return true. (Length 1: no neck, every turn is valid.)
    return false;
}

void Snake::reverse()
{
    // TODO: Reverse the body. New direction: from the new second segment towards the new head.
    //   e.g. head (2,1), second (2,2)  -> LEFT
    // If the snake has length 1, the direction just becomes the opposite.
}

void Snake::getNextHeadPosition(int &row, int &col) const
{
    // TODO: The cell the head would move into with the current direction.
}
