#ifndef _KING_H_
#define _KING_H_

#include <bits/stdc++.h> 
#include <iostream>
#include <fstream>
#include "board.h"
#include "position.h"
#include "vector"

class King {
    private:
       unsigned static long getNextMoves(Board&, bool, int);
    public:
        King(/* args */);
        ~King();
        static void getAllKingMoves(Board&, std::vector<std::pair<int,int>>&, bool); 
        static bool isKingAttacked(Board&, bool, int);
        static void checkCastle(Board& , int , unsigned long &, bool);
        static bool moveInCheck(Board&, bool, int, std::unordered_set<int>&);
};

#endif // 
