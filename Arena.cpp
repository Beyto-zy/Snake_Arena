#include "Arena.h"
#include <iostream>
#include <sstream>
#include <cctype>

Arena::Arena() : firstSnake(nullptr), lastSnake(nullptr), tickCount(0) {}

Arena::~Arena()
{
    clearSnakes();
    // TODO: Free all remaining snakes.
}

// ---------------------------------------------------------------- helpers (given)

// A valid snake id is a single uppercase letter A-Z.
static bool isSnakeId(const std::string &s)
{
    return s.size() == 1 && s[0] >= 'A' && s[0] <= 'Z';
}

// ---------------------------------------------------------------- player list

Snake *Arena::findSnake(char id) const
{
    Snake* current = firstSnake;
    while(current != nullptr){
        if(current->isAlive() && current->getId() == id){
            return current;
        }
        current = current->getNext();
    }
    // TODO: Return the ALIVE snake with this id, or nullptr.
    return nullptr;
}

void Arena::addSnake(Snake *s)
{   
    if(s == nullptr){
        return;
    }
    s->setNext(nullptr);

    if(firstSnake == nullptr){
        firstSnake = s;
        lastSnake = s;
    }
    else{
        lastSnake->setNext(s);
        lastSnake = s;
    }
    // TODO: Append s to the end of the player list (turn order = spawn order).
}

void Arena::clearSnakes()
{   
    if(firstSnake == nullptr){
        return;
    }
    else{
        Snake* curr = firstSnake;
        while(curr != nullptr){
            Snake* nextSnake = curr->getNext();
            delete curr;
            curr = nextSnake;
        }
        firstSnake = nullptr;
        lastSnake = nullptr;
        leaderboard.clear();
        
    }
    // TODO: Delete every snake in the player list, reset the list
    // and clear the leaderboard.
}

void Arena::removeDeadSnakes()
{   
    Snake* current = firstSnake;
    Snake* prev = nullptr;

    while(current != nullptr){
        if(!current->isAlive()){
            Snake* temp = current;
            if(prev == nullptr){
                firstSnake = current->getNext();
            }
            else{
                prev->setNext(current->getNext());
            }
            if(temp == lastSnake){
                lastSnake = prev;
            }
            current = current->getNext();
            delete temp;
        }
        else{
            prev =current;
            current = current->getNext();
        }
    }
    // TODO: Unlink and delete every snake with isAlive() == false.
}

void Arena::killSnake(Snake *s){
    if (s == nullptr || !s->isAlive()){
        return;
    }

    std::cout << "Snake " << s->getId() << " died with length " << s->getBody().getLength() << " and score " << s->getScore() << "." << std::endl;

    Segment* curr = s->getBody().getHead();
    while(curr != nullptr){
        board.set(curr->getRow(), curr->getCol(), FOOD_CELL);
        curr = curr->getNext();
    }

    leaderboard.remove(s);
    s->setAlive(false);


    // TODO:
    //  - Print: std::cout << "Snake " << id << " died with length " << length
    //           << " and score " << score << "." << std::endl;
    //  - Every cell of its body becomes FOOD_CELL on the board.
    //  - Mark it dead and remove it from the leaderboard.
    //  Note: tick() may still be iterating over the player list.
}

// ---------------------------------------------------------------- movement

void Arena::moveSnake(Snake *s){
    int nr;
    int nc;

    s->getNextHeadPosition(nr, nc);

    if(!board.inBounds(nr, nc) || board.get(nr, nc) == WALL_CELL){
        std::cout << "Snake " << s->getId() << " hit a wall at (" << nr << "," << nc << ")." << std::endl;
        killSnake(s);
        return;
    }

    char target = board.get(nr, nc);

    Segment* tail = s->getBody().getTail();
    bool isOwnTail = (tail != nullptr && tail->getRow() == nr && tail->getCol() == nc);

    if(target == tolower(s->getId()) && !isOwnTail){
        std::cout << "Snake " << s->getId() << " bit itself at (" << nr << "," << nc << ")." << std::endl;
        killSnake(s);
        return;
    }

    if(isupper(target) && target != s->getId()){
        Snake* t = findSnake(target);
        std::cout << "Snake " << s->getId() << " and Snake " << target << " collided head-on at (" << nr << "," << nc << ")." << std::endl;

        int lenS = s->getBody().getLength();
        int lenT = t->getBody().getLength();
        
        if(lenS > lenT){
            killSnake(t);
            Segment* oldHead = s->getBody().getHead();
            if(oldHead != nullptr){
                board.set(oldHead->getRow(), oldHead->getCol(), tolower(s->getId()));
            }
            s->getBody().pushFront(nr,nc);
            board.set(nr,nc,s->getId());
            s->addScore(1);
            leaderboard.reposition(s);
            std::cout << "Snake " << s->getId() << " ate food at (" << nr << "," << nc << ")." << std::endl;
        }
        else if(lenS < lenT){
            killSnake(s);
        }
        else{
            killSnake(s);
            killSnake(t);
        }

        return;
    }

    if(islower(target) && target != tolower(s->getId())){
        char otherId = static_cast<char>(toupper(target));
        Snake* t = findSnake(otherId);
    
        if(t != nullptr){
            SnakeBody cutPart = t->getBody().splitAt(nr, nc);
            int k = cutPart.getLength();

            Segment* seg = cutPart.getHead();
            while (seg != nullptr){
                board.set(seg->getRow(), seg->getCol(), FOOD_CELL);
                seg = seg->getNext();                
            }
            std::cout << "Snake " << s->getId() << " bit Snake " << otherId << " at (" << nr << "," << nc << "). Snake " << otherId << " lost " << k << " segment(s)." << std::endl;

            Segment* oldhead = s->getBody().getHead();
            if(oldhead != nullptr){
                board.set(oldhead->getRow(), oldhead->getCol(), tolower(s->getId()));
            }
            s->getBody().pushFront(nr,nc);
            board.set(nr,nc, s->getId());
            s->addScore(1);
            leaderboard.reposition(s);
            std::cout << "Snake " << s->getId() << " ate food at (" << nr << "," << nc << ")." << std::endl;
            return;
        }
    }
        if(target == FOOD_CELL){
            Segment* oldHead = s->getBody().getHead();
            if(oldHead != nullptr){
                board.set(oldHead->getRow(), oldHead->getCol(), tolower(s->getId()));
            }
            s->getBody().pushFront(nr, nc);
            board.set(nr,nc, s->getId());
            s->addScore(1);
            leaderboard.reposition(s);
            std::cout << "Snake " << s->getId() << " ate food at (" << nr << "," << nc << ")." << std::endl;
            return;   
        }

        if (target == EMPTY_CELL || isOwnTail){
            int tr;
            int tc;
            s->getBody().popBack(tr,tc);
            board.set(tr,tc,EMPTY_CELL);

            Segment* oldHead = s->getBody().getHead();
            if(oldHead != nullptr){
                board.set(oldHead->getRow(), oldHead->getCol(), tolower(s->getId()));
            }

            s->getBody().pushFront(nr,nc);
            board.set(nr,nc, s->getId());
            return;
        }
    // TODO: Move s one cell forward. The rules and messages are in Section 4.6 of the assignment.
    // Messages:
    //   "Snake A hit a wall at (nr,nc)."
    //   "Snake A and Snake B collided head-on at (nr,nc)."
    //   "Snake A bit itself at (nr,nc)."
    //   "Snake A bit Snake B at (nr,nc). Snake B lost K segment(s)."
    //   "Snake A ate food at (nr,nc)."
    // The board must always show the head as the uppercase id, body as lowercase.
}

void Arena::tick()
{
    tickCount++;
    std::cout << "--- Tick " << tickCount << " ---" << std::endl;

    Snake* current = firstSnake;
    while(current != nullptr){
        if(current->isAlive()){
            moveSnake(current);
        }
        current = current->getNext();
    }
    removeDeadSnakes();
    // TODO:
    //  - increase tickCount and print "--- Tick <tickCount> ---"
    //  - every alive snake moves once, in player list order
    //  - snakes that died during this tick must not move
    //  - dead snakes must be removed from the player list before the tick ends
}

// ---------------------------------------------------------------- commands

void Arena::cmdInitBoard(const std::string &args)
{
    std::istringstream ss(args);
    int r, c;
    ss >> r >> c;

    leaderboard.clear();
    tickCount = 0;
    board = Board(r,c);
    clearSnakes();

    std::cout << "Board initialized with " << r << " rows and " << c << " columns." << std::endl;

    // TODO: INIT_BOARD <rows> <cols>   (the input is always well-formed)
    //  Resets everything: snakes, leaderboard, tick counter.
    //  Create the new board with move assignment:  board = Board(r, c);
    //  "Board initialized with <r> rows and <c> columns."
}

// Given ready, as an example for parsing.
// (Checking the format is not required: the input is always well-formed.)
void Arena::cmdWall(const std::string &args)
{
    std::istringstream ss(args);
    int r, c;
    ss >> r >> c;
    if (!board.inBounds(r, c) || board.get(r, c) != EMPTY_CELL)
    {
        std::cout << "Error: Cell (" << r << "," << c << ") is not available." << std::endl;
        return;
    }
    board.set(r, c, WALL_CELL);
    std::cout << "Wall placed at (" << r << "," << c << ")." << std::endl;
}

void Arena::cmdFood(const std::string &args)
{   
    std::istringstream ss(args);
    int r, c;
    ss >> r >> c;
    if(!board.inBounds(r,c) || board.get(r, c) != EMPTY_CELL){
        std::cout << "Error: Cell (" << r << "," << c << ") is not available." << std::endl;
        return;
    }
    else{
        board.set(r,c,FOOD_CELL);
        std::cout << "Food placed at (" << r << "," << c << ")." << std::endl;
    }
    // TODO: FOOD <r> <c>   (same rules as WALL)
    //  "Error: Cell (r,c) is not available."
    //  "Food placed at (r,c)."
}

void Arena::cmdSpawn(const std::string &args)
{
    std::stringstream ss(args);
    char id;
    int r, c, len;
    std::string dirStr;

    ss >> id >> r >> c >> dirStr >> len;

    if (findSnake(id) != nullptr) {
        std::cout << "Error: Snake " << id << " already exists." << std::endl;
        return;
    }  

    Direction dir;
    parseDirection(dirStr, dir);
    Direction opp = oppositeDirection(dir);

    int dr = rowDelta(opp);
    int dc = colDelta(opp);
    for(int i = 0; i < len; i++){
        int curR = r + i * dr;
        int curC = c + i * dc;
        if(!board.inBounds(curR, curC) || board.get(curR, curC) != EMPTY_CELL){
            std::cout << "Error: Cannot spawn Snake "<< id <<" at ("<< r << "," << c << ")."<< std::endl;
            return;
        }
    }    
    Snake* snake = new Snake(id, dir);
    for(int i = 0; i < len;i++){
        int curR = r+i * dr;
        int curC = c+i * dc;
        snake->getBody().pushBack(curR, curC);
        char ch;
        if (i == 0){
            ch = id;              
        } 
        else{
            ch = tolower(id);        
        }
        board.set(curR, curC, ch);
    }
    addSnake(snake);
    leaderboard.insertSorted(snake);
    std::cout << "Snake " << id << " spawned at (" << r << "," << c << ") facing " << directionToString(dir) << " with length " << len << "." << std::endl;
    // TODO: SPAWN <id> <r> <c> <dir> <len>
    //  The head is at (r,c), the body extends in the OPPOSITE direction of <dir>.
    //  alive snake with same id -> "Error: Snake <id> already exists."
    //  any cell out of bounds or not EMPTY_CELL -> "Error: Cannot spawn Snake <id> at (r,c)."
    //  Add to the player list AND to the leaderboard.
    //  "Snake <id> spawned at (r,c) facing <DIR> with length <len>."
}

void Arena::cmdTurn(const std::string &args)
{
    std::istringstream ss(args);
    char id;
    std::string dirStr;
    Direction dir;
    ss >> id >> dirStr;
    parseDirection(dirStr, dir);

    Snake* snake = findSnake(id);
    if(findSnake(id) == nullptr){
        std::cout << "Error: Snake " << id << " not found." << std::endl;
        return;
    }
    else{
        if(!snake->turn(dir)){
            std::cout << "Error: Snake " << id << " cannot turn back." << std::endl;
        }
        else{
            std::cout << "Snake " << id << " is now facing " << directionToString(dir) << "." << std::endl;
        }
    }
    // TODO: TURN <id> <dir>
    //  "Error: Snake <id> not found."
    //  "Error: Snake <id> cannot turn back."
    //  "Snake <id> is now facing <DIR>."
}

void Arena::cmdReverse(const std::string &args){
    std::istringstream ss(args);
    char id;
    ss >> id;
    Snake* snake = findSnake(id);
    
    if (snake == nullptr){
        std::cout << "Error: Snake " << id << " not found." << std::endl;
        return;
    }
    if (snake == nullptr) {
        std::cout << "Error: Snake " << id << " not found." << std::endl;
        return;
    }
    Segment* oldHead = snake->getBody().getHead();
    board.set(oldHead->getRow(), oldHead->getCol(), tolower(id));

    snake->reverse();

    Segment* newHead = snake->getBody().getHead();
    board.set(newHead->getRow(), newHead->getCol(), id);

    std::cout << "Snake " << id << " reversed and is now facing " << directionToString(snake->getDirection()) << "." << std::endl;
    // TODO: REVERSE <id>
    //  "Error: Snake <id> not found."
    //  Update the board too (old head becomes lowercase, new head uppercase).
    //  "Snake <id> reversed and is now facing <DIR>."
}

void Arena::cmdTick(const std::string &args){
    std::istringstream ss(args);
    int n = 1;
    ss >> n;
    for (int i = 0; i < n; i++){
        tick();
    } 
    // TODO: TICK [n]   (n is optional, default 1)
}

// Given ready
void Arena::printSnakes() const
{
    std::cout << "--- Snakes ---" << std::endl;
    if (!firstSnake)
    {
        std::cout << "No snakes in the arena." << std::endl;
        return;
    }
    for (Snake *cur = firstSnake; cur; cur = cur->getNext())
    {
        std::cout << cur->getId() << " " << directionToString(cur->getDirection())
                  << " length=" << cur->getLength() << " score=" << cur->getScore()
                  << ": " << cur->getBody() << std::endl;
    }
}

// Given ready
void Arena::processCommand(const std::string &line)
{
    std::istringstream ss(line);
    std::string cmd;
    if (!(ss >> cmd))
        return;
    std::string args;
    std::getline(ss, args);

    if (cmd == "INIT_BOARD")
    {
        cmdInitBoard(args);
        return;
    }

    bool knownBoardCmd = cmd == "WALL" || cmd == "FOOD" || cmd == "SPAWN" || cmd == "TURN" ||
                         cmd == "REVERSE" || cmd == "TICK" || cmd == "PRINT_BOARD" ||
                         cmd == "PRINT_SNAKES" || cmd == "PRINT_LEADERBOARD";
    if (!knownBoardCmd)
    {
        std::cout << "Error: Unknown command '" << cmd << "'." << std::endl;
        return;
    }
    if (!board.isInitialized())
    {
        std::cout << "Error: Board not initialized." << std::endl;
        return;
    }

    if (cmd == "WALL")
        cmdWall(args);
    else if (cmd == "FOOD")
        cmdFood(args);
    else if (cmd == "SPAWN")
        cmdSpawn(args);
    else if (cmd == "TURN")
        cmdTurn(args);
    else if (cmd == "REVERSE")
        cmdReverse(args);
    else if (cmd == "TICK")
        cmdTick(args);
    else if (cmd == "PRINT_BOARD")
        board.print();
    else if (cmd == "PRINT_SNAKES")
        printSnakes();
    else if (cmd == "PRINT_LEADERBOARD")
        leaderboard.print();
}
