
/*SuperTac is a video-game which is quite like tic-tac-toe, albeit much
more flexible. One is capable of setting up a board to a given size, or at
an infinite size in SuperTac, and additionally changing the number of
consecutive symbols required to obtain a tic-tac-toe*/

#include <iostream>
#include <iterator>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <set>

struct Coord{
    long x, y;

    Coord(long xi, long yi){
        x = xi;
        y = yi;
    }

    bool operator == (const Coord& i) const{
        return (i.x == x) && (i.y == y);
    }
};

struct CoordHash{
    public:
    std::size_t operator()(const Coord& c) const{

        size_t base = 17;
        base = base * 31 + std::hash<long>()(c.x);
        base = base * 31 + std::hash<long>()(c.y);

        return base;
    }
};

class Board{
    int ID;
    bool won;
    std::unordered_map<Coord, char, CoordHash> board;
    public:
        int winCond;
        void initializeBoard();
        int setChar(long, long, char);
        bool checkChar(long, long, char);
};

void Board::initializeBoard(){
    ID = 0;
    std::cout << "Enter number of consecutive tokens needed for victory" << std::endl;
    std::cin >> winCond;
}

bool Board::checkChar (long x, long y, char c){
    Coord point(x, y);
    if(board.count(point) && board[point] == c){
        return true;
    }
    return false;
}

int Board::setChar (long i, long j, char c){
    Coord point(i, j);
    if(!board.count(point)){
        board.emplace(point, c);
        return 0;
    } else return -1;
}

bool horizCheck(Board playfield, long x, long y, char c){
    int condition = playfield.winCond - 1;
    if(condition == 0) return true;
    int tempX = x;
    while(playfield.checkChar(tempX - 1, y, c)){
        condition--;
        if(condition == 0) return true;
        tempX--;
    }
    tempX = x;
    while(playfield.checkChar(tempX + 1, y, c)){
        condition--;
        if(condition == 0) return true;
        tempX++;
    }
    return false;
}

bool vertCheck(Board playfield, long x, long y, char c){
    int condition = playfield.winCond - 1;
    if(condition == 0) return true;
    int tempY = y;
    while(playfield.checkChar(x, tempY - 1, c)){
        condition--;
        if(condition == 0) return true;
        tempY--;
    }
    tempY = y;
    while(playfield.checkChar(x, tempY + 1, c)){
        condition--;
        if(condition == 0) return true;
        tempY++;
    }
    return false;
}

bool diag1Check(Board playfield, long x, long y, char c){
    int condition = playfield.winCond - 1;
    if(condition == 0) return true;
    int tempY = y;
    int tempX = x;
    while(playfield.checkChar(tempX - 1, tempY - 1, c)){
        condition--;
        if(condition == 0) return true;
        tempY--;
        tempX--;
    }
    tempY = y;
    tempX = x;
    while(playfield.checkChar(tempX + 1, tempY + 1, c)){
        condition--;
        if(condition == 0) return true;
        tempY++;
        tempX++;
    }
    return false;
}

bool diag2Check(Board playfield, long x, long y, char c){
    int condition = playfield.winCond - 1;
    if(condition == 0) return true;
    int tempY = y;
    int tempX = x;
    while(playfield.checkChar(tempX + 1, tempY - 1, c)){
        condition--;
        if(condition == 0) return true;
        tempY--;
        tempX++;
    }
    tempY = y;
    tempX = x;
    while(playfield.checkChar(tempX - 1, tempY + 1, c)){
        condition--;
        if(condition == 0) return true;
        tempY++;
        tempX--;
    }
    return false;
}

bool checkWinCond(Board playfield, long x, long y, char c){
    if(horizCheck(playfield, x, y, c)) return true;
    if(vertCheck(playfield, x, y, c)) return true;
    if(diag1Check(playfield, x, y, c)) return true;
    if(diag2Check(playfield, x, y, c)) return true;
    return false;
}

class Player{
    public:
        unsigned short ID;
        std::string name;
        char symbol;
        void initializePlayer();
        int executeTurn(Board&, int);
};

void Player::initializePlayer(){
    std::cout << "Enter Player's Name" << std::endl;
    std::cin >> name;
    std::cout << "Enter Player's Symbol" << std::endl;
    symbol = getchar();
}

int Player::executeTurn(Board& playfield, int turnNumber){
    long x , y;
    while(true){
        std::cout << "Enter Symbol Coordinates for " << name << std::endl;
        std::cin >> x >> y;
        if(!playfield.setChar(x, y, symbol)){
            break;
        } else std::cout << "Error, space already occupied" << std::endl;
    }
    if(turnNumber < playfield.winCond) return 0;
    return checkWinCond(playfield, x, y, symbol);
}

std::vector<Player> generatePlayers(std::set<char>& library){
    std::vector<Player> list;
    std::set<std::string> names;
    bool started = false;
    while(true){
        std::string name;
        char type;
        while(true){
            std::cout << "Enter a Player name" << std::endl;
            std::cin >> name;
            if(names.count(name)){
                std::cout << "Invalid Symbol entered." << std::endl;
            }else{
               names.insert(name);
               break;
            }
        }
        while(true){
            std::cout << "Enter " << name << "'s symbol" << std::endl;
            std::cin >> type;
            if(library.count(type)){
                std::cout << "Invalid Symbol entered." << std::endl;
            }else{
               library.insert(type);
               break;
            }
        }
        Player found;
        found.name = name;
        found.symbol = type;
        list.push_back(found);
        if(started){
            std::cout << "Add another player? 0 for no, any other number for yes" << std::endl;
            int valid;
            std::cin >> valid;
            if(!valid){
                break;
            }
        }
        started = true;
    }
    return list;
}

int main(){
    Board game;
    game.initializeBoard();
    std::set<char> symbolLibrary = {0};
    std::vector<Player> playerList = generatePlayers(symbolLibrary);
    bool won = false;
    bool ended = false;
    while(!ended){
        int turnNumber = 1;
        while(!won){
            for(unsigned int i = 0; i < playerList.size(); i++){
                if(playerList[i].executeTurn(game, turnNumber)){
                    std::cout << playerList[i].name << " has won!" << std::endl;
                    won = true;
                    break;
                }
            }
            turnNumber++;
        }
        std::cout << "Would you like to go again? 0 for no, anything else for yes" << std::endl;
        int resp;
        std::cin >> resp;
        delete(&game);
        if(resp){
            for (Player player : playerList){
                delete(&player);
            }
            break;
        }
        Board game;
        won = false;
        game.initializeBoard();
    }
    return 0;
}








