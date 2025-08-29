#pragma once
#include <limits>

#define RESET "\033[0m"
#define RED "\033[1m\033[31m"
#define GREEN "\033[1m\033[32m"
#define DEPTH 32  // how far the computer searches in the game
#define INFINITY std::numeric_limits<double>::infinity() // Can replace this

typedef enum{
    PLAYER,
    COMPUTER,
    DRAW,
    GOING
} state;

class game{
    private:
        char board[9]{'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    public:
        const std::string&  color(char c);
        void                print();
        void                makeMove(int pos, char symbol);
        void                unmakeMove(int pos); 
        void                isGameOver();
        state               checkWin();
        char                isMoveValid(int pos);
        char                grabCell(int pos);
};

class inheritance{
    protected:
        game& instance;
    public:
        inheritance(game& instance)
            : instance{instance}
        {}
};

class player : public inheritance{
    private:
        static const char symbol{'X'};
    public:
        player(game& instance)
            : inheritance{instance}
        {}
        static char     getSymbol();
        void            makeMove();
};

class computer : public inheritance{
    private:
        static const char symbol{'O'};
        int iterations{};
        int counter{};
    public:
        computer(game& instance)
            : inheritance{instance}
        {}
        static char     getSymbol();
        void            makeMove();
        int             Minimax(int depth, bool isMax);
        int             MinimaxABP(int depth, bool Maximizing, int alpha, int beta);
};

/*
std::ostream& operator<<(std::ostream& stream, char& chr)
{
    if(chr == 'O')
    {
        stream << RED << 'O' << RESET;
        return stream;
    }
    else if(chr == 'X')
    {
        stream << GREEN << 'X' << RESET;
        return stream;
    }
    else
    {
        if(isdigit(chr))
        {
            stream << chr - '0';
            return stream;
        }
        else
        {
            stream << chr;
            return stream;
        }
    }
}*/
