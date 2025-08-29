#include <iostream>
#include "game_r.hpp"

// g++ main.cpp -Werror -Wall -pedantic -o game -std=c++17

const std::string& game::color(char c){
    static const std::string player     = std::string(GREEN) + 'X' + RESET;
    static const std::string computer   = std::string(RED)   + 'O' + RESET;
    static const std::string digits[] = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };
    switch(c){
        case 'X':{
            return player;
        }
        case 'O':{
            return computer;
        }
        default:{
            return digits[c - '0'  - 1];
        }
    }
}

void game::print(){
    std::cout << 
    "    |     |    \n"
    " " << color(board[0]) << "  |  " << color(board[1]) << "  |  " << color(board[2]) << "\n"
    "    |     |    \n"
    "----+-----+------\n"
    "    |     |    \n"
    " " << color(board[3]) << "  |  " << color(board[4]) << "  |  " << color(board[5]) << "\n"
    "    |     |    \n"
    "----+-----+------\n"
    "    |     |    \n"
    " " << color(board[6]) << "  |  " << color(board[7]) << "  |  " << color(board[8]) << "\n"
    "    |     |    \n\n";
    return;
}

void game::makeMove(int pos, char symbol){
    board[pos] = symbol;
    return;
}

void game::unmakeMove(int pos){
    board[pos] = pos + '0' + 1;
    return;
}

state game::checkWin(){
    static const int winPatterns[8][3] = {
        {0, 1, 2},  // Row 1
        {3, 4, 5},  // Row 2
        {6, 7, 8},  // Row 3
        {0, 3, 6},  // Col 1
        {1, 4, 7},  // Col 2
        {2, 5, 8},  // Col 3
        {0, 4, 8},  // Diagonal
        {2, 4, 6}   // Diagonal
    };
    for(const int (&win)[3] : winPatterns){
        char a = board[win[0]];
        char b = board[win[1]];
        char c = board[win[2]];
        if(a == b && b == c){
            if(a == 'X'){ return PLAYER; }
            if(a == 'O'){ return COMPUTER; }
        }
    }
    for(char pos : board){
        if( isdigit(pos) ){
            return GOING;
        }
    }
    return DRAW;
}

void game::isGameOver(){
    state status = game::checkWin();
    switch(status){
        case PLAYER:{
            game::print();
            std::cout << "Player has won!\n";
            exit(EXIT_SUCCESS);
        }
        case COMPUTER:{
            game::print();
            std::cout << "Computer has won!\n";
            exit(EXIT_SUCCESS);
        }
        case DRAW:{
            game::print();
            std::cout << "Draw!\n";
            exit(EXIT_SUCCESS);
        }
        default:{
            return;
        }
    }
}

char game::isMoveValid(int pos){
    return  ( (board[pos] != 'X') &&
              (board[pos] != 'O') );
}

char game::grabCell(int pos) { return board[pos]; }

char player::getSymbol()    { return symbol; }
char computer::getSymbol()  { return symbol; }

void player::makeMove(){
    start:
    int num{};
    std::cout << "Enter a number 1-9: ";std::cin >> num;
    if(std::cin.fail()){
        goto error;
    }

    num -= 1;
    if(! instance.isMoveValid(num) ){
        error:
        std::cout << "Wrong move please try again!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        goto start;
    }
    instance.makeMove(num, symbol);
    instance.isGameOver();
    /*
    * now here is do what do i do abotu the game::makeMove()
    */
   return;
}

int computer::Minimax(int depth, bool isMax){
    state status = instance.checkWin(); 

    if(status != GOING || depth == DEPTH){
        ++iterations;
        if(status == COMPUTER){     // Computer
            return 10 - depth;
        }
        else if(status == PLAYER){  // Player
            return -10 + depth;
        }
        else{                       // Draw
            return 0;
        }
    }
    if(isMax){                      // Computer simulation
        int Best = -INFINITY;
        for(int i = 0; i < 9; ++i){
            if( isdigit( instance.grabCell(i) ) ){
                instance.makeMove(i, computer::getSymbol());
                Best = std::max(Best, this->Minimax(depth + 1, false));
                instance.unmakeMove(i);
            }
        }
        return Best;
    }
    else{                           // Player simulation
        int Best = INFINITY;
        for(int i = 0; i < 9; ++i){
            if( isdigit( instance.grabCell(i) ) ){
                instance.makeMove(i, player::getSymbol());
                Best = std::min(Best, this->Minimax(depth + 1, true));
                instance.unmakeMove(i);
            }
        }
        return Best;
    }
}

int computer::MinimaxABP(int depth, bool Maximizing, int alpha, int beta){
    state status = instance.checkWin();

    if(status != GOING || depth == DEPTH){
        ++iterations;
        if(status == COMPUTER){     // Computer
            return 10 - depth;
        }
        else if(status == PLAYER){  // Player
            return -10 + depth;
        }
        else{                       // Draw
            return 0;
        }
    }
    if(Maximizing){                 // Computer simulation
        int Best = -INFINITY;
        for(int i = 0; i < 9; ++i){
            if( isdigit( instance.grabCell(i) ) ){
                instance.makeMove(i, computer::getSymbol());
                Best = std::max(Best, this->MinimaxABP(depth + 1, false, alpha, beta));
                instance.unmakeMove(i);
                alpha = std::max(alpha, Best);
                if(beta <= alpha){
                    break;
                }
            }
        }
        return Best;
    }
    else{                           // Player simulation
        int Best = INFINITY;
        for(int i = 0; i < 9; ++i){
            if( isdigit( instance.grabCell(i) ) ){
                instance.makeMove(i, player::getSymbol());
                Best = std::min(Best, this->MinimaxABP(depth + 1, true, alpha, beta));
                instance.unmakeMove(i);
                beta = std::min(beta, Best);
                if (beta <= alpha){
                    break;
                }
            }
        }
        return Best;
    }
}


void computer::makeMove(){
    int MaxScore = -INFINITY;
    int Index{};
    for(int i = 0; i < 9; ++i){
        if( isdigit( instance.grabCell(i) ) ){
            instance.makeMove(i, computer::getSymbol());
            int Score = this->Minimax(0, false);
            //int Score = this->MinimaxABP(0, false, -INFINITY, INFINITY);
            instance.unmakeMove(i);
            if(Score > MaxScore){
                MaxScore = Score;
                Index = i;
            }
        }
    }
    std::cout << "\033[1m\033[33mCPU has searched " << "\033[1m\033[36m" << iterations << "\033[0m" << " \033[1m\033[33mmoves total\033[0m" << "\n\n";
    iterations = 0;
    instance.makeMove(Index, computer::getSymbol());
    ++counter;
    instance.isGameOver();
    return;
}


int main(){
    game        tictactoe{};
    computer    machine{tictactoe};
    player      human{tictactoe};

    char choice;
    std::cout << "Do you want to go first[Y/N]: ";std::cin >> choice;
    switch(tolower(choice)){
        case 'n':{
            while(true){
                machine.makeMove();
                tictactoe.print();
                human.makeMove();
            }
            break;
        }
        case 'y':{
            while(true)
            {
                tictactoe.print();
                human.makeMove();
                machine.makeMove();
            }
            break;
        }
        default:{
            std::cout << "Pick an option...\n";
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
