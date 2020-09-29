#include <iostream>
#include <fstream>
#include "board.h"
#include "position.h"
#include "vector"
#ifndef _PAWN_H_
#define _PAWN_H_
class Pawn
{
    private:
       static unsigned long getNextMoves(Board&, bool, int);
    public:
        Pawn(/* args */);
        ~Pawn();
        static void getAllPawnMoves(Board&, std::vector<std::pair<int,int>>&, bool, std::ofstream& miniFile);
};

#endif // !_PAWN_H_
