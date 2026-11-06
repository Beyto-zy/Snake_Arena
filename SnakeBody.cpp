#include "SnakeBody.h"

SnakeBody::~SnakeBody() { clear(); }

SnakeBody::SnakeBody(SnakeBody &&other) noexcept
    : head(nullptr), tail(nullptr), length(0)
{
    this->head = other.head;
    this->tail = other.tail;
    this->length = other.length;

    other.head = nullptr;
    other.tail = nullptr;
    other.length = 0;

    // TODO: Take over other's segments. 'other' must be left empty but valid.
}

SnakeBody &SnakeBody::operator=(SnakeBody &&other) noexcept
{   
    if (this != &other){
        clear();
        this->head = other.head;
        this->tail = other.tail;
        this->length = other.length;

        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;
    }
    // TODO: Same as the move constructor, but this body may already own segments
    // (they must not leak).
    return *this;
}

bool SnakeBody::isEmpty() const
{
    // TODO: true if the body has no segments.
    if (length == 0){
        return true;
    }
    else{
        return false;
    }
}

void SnakeBody::pushFront(int row, int col)
{
    Segment* newSeg = new Segment(row,col);
    if (head == nullptr){
        head = newSeg;
        tail = newSeg;
    }
    else{
        newSeg->next = head;
        head->prev = newSeg;
        head = newSeg;
    }
    length++;
    // TODO: Add a new segment at (row,col) as the new head.
}

void SnakeBody::pushBack(int row, int col)
{   
    Segment* newSeg = new Segment(row,col);
    if(head == nullptr){
        head = newSeg;
        tail = newSeg;        
    }
    else{
        newSeg->prev = tail;
        newSeg->next = nullptr;
        tail->next = newSeg;
        tail = newSeg;
    }
    length++;
    // TODO: Add a new segment at (row,col) as the new tail.
}

bool SnakeBody::popBack(int &row, int &col)
{   
    if (head == nullptr){
        return false;
    }
    
    row = tail->row;
    col = tail->col;

    if(head == tail){
        delete tail;
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    else{
    Segment* tempPointer = tail;
    tail = tail->prev;
    tail->next = nullptr;
    delete tempPointer;
    length--;
    }
    
    // TODO: Remove the tail segment, write its coordinates into row/col and delete it.
    // Return false if the body is empty.
    return true;
}

void SnakeBody::reverse()
{
    if ( head == nullptr || head == tail){
        return;
    }
    Segment* current = head;
    while (current != nullptr){
        Segment* temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        current = temp;
    }
    Segment* temp = head;
    head = tail;
    tail = temp;
    // TODO: Reverse the list in place (do not create or delete segments).
}

Segment *SnakeBody::findAt(int row, int col) const
{   
    Segment* current = head;

    while (current != nullptr){
        if(current->row == row && current->col == col){
            return current;
        }
        current = current->next;
    }
    // TODO: Return the segment at (row,col), or nullptr if there is none.
    return nullptr;
}

SnakeBody SnakeBody::splitAt(int row, int col)
{
    SnakeBody cut; // returned by value -> moved, not copied
    Segment* target = findAt(row,col);

    if(target == nullptr){
        return cut;
    }

    if(target == head){
        cut.head = head;
        cut.tail = tail;
        cut.length = length;
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    else{
        Segment* prevSeg = target->prev;
        prevSeg->next = nullptr;
        target->prev = nullptr;
        cut.head = target;
        cut.tail = this->tail;
        this->tail = prevSeg;

        int counter = 0;
        Segment* current = cut.head;
        while(current != nullptr) {
            counter++;
            current = current->next;
        }
        cut.length = counter;
        length = length - counter;
    }
    // TODO: The segment at (row,col) and every segment after it (towards the tail)
    // are moved into 'cut'; this body keeps the segments before it.
    // If there is no segment at (row,col), return an empty body.

    return cut;
}

void SnakeBody::clear() {   
    while (head != nullptr) {
        Segment* nextNode = head->next;
        delete head;
        head = nextNode;
    }
    tail = nullptr;
    length = 0;
    // TODO: Delete every segment. The body must be empty and usable afterwards.
}

// Given ready
std::ostream &operator<<(std::ostream &os, const SnakeBody &b)
{
    for (Segment *cur = b.head; cur; cur = cur->getNext())
    {
        os << *cur;
        if (cur->getNext())
            os << " ";
    }
    return os;
}
