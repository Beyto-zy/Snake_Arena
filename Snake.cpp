#include "Snake.h"

Snake::Snake(char _id, Direction _direction)
    : id(_id), direction(_direction), score(0), alive(true), next(nullptr) {}

bool Snake::turn(Direction newDir)
{
    if (body.getLength() <= 1){
        direction = newDir;
        return true;
    }
    else{
        const Segment* headSegment = body.getHead();
        const Segment* neck = headSegment->getNext();

        int targetRow = headSegment->getRow() + rowDelta(newDir);
        int targetCol = headSegment->getCol() + colDelta(newDir);

        if (targetRow == neck->getRow() && targetCol == neck->getCol()){
            return false;
        }
        else{
            direction = newDir;
            return true;
        }


    }
    // TODO: The second segment is the "neck". If moving one cell in newDir would put
    // the head onto the neck, return false and change nothing. Otherwise set the
    // direction and return true. (Length 1: no neck, every turn is valid.)
}

void Snake::reverse()
{
    body.reverse();

    if (body.getLength() <= 1){
        direction = oppositeDirection(direction);
    }
    else{
        const Segment* headSegment = body.getHead();
        const Segment* second = body.getHead()->getNext();

        if(headSegment->getRow() < second->getRow()){
            //Up
            direction = Direction::UP;
        }
        else if(headSegment->getRow() > second->getRow()){
            //Down
            direction = Direction::DOWN;
        }
        else if(headSegment->getCol() < second->getCol()){
            //Left
            direction = Direction::LEFT;
        }
        else if(headSegment->getCol() > second->getCol()){
            //Right
            direction = Direction::RIGHT;
        }
    }


    // TODO: Reverse the body. New direction: from the new second segment towards the new head.
    //   e.g. head (2,1), second (2,2)  -> LEFT
    // If the snake has length 1, the direction just becomes the opposite.
}

void Snake::getNextHeadPosition(int &row, int &col) const
{
    const Segment* headSegment= body.getHead();

    int currentHeadRow = headSegment->getRow();
    int currentHeadCol = headSegment->getCol();

    row = currentHeadRow + rowDelta(direction);
    col = currentHeadCol + colDelta(direction);

    // TODO: The cell the head would move into with the current direction.
}
