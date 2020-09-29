#include <bits/stdc++.h>
#include <fstream>
#include "board.h"
#include "position.h"

#ifndef _QUEEN_H_
#define _QUEEN_H_

class Queen
{
    private:

    public:
        Queen(/* args */);
        ~Queen();
        static void getAllQueenMoves(Board&, std::vector<std::pair<int, int>>&, bool);
};

#endif //