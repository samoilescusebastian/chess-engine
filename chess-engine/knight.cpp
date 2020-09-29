#include <iostream>
#include <fstream>
#include "board.h"
#include "position.h"
#include "knight.h"
#include "constants.h"

Knight::Knight(/* args */)
{
}

Knight::~Knight()
{
}

void Knight::getAllKnightMoves(Board &board, std::vector<std::pair<int, int>> &v, bool player) {
    unsigned long knights = board.getBitMap(KNIGHT, player);
    unsigned long result = 0;
    for (int i = 0; i < POSITIONS; i++) {
        if ((knights & (board.masks[i])) != 0) {
            result = Knight::getNextMove(board, player, i);
            for (int j = 0; j < POSITIONS; j++) {
                if ((result & (board.masks[j])) != 0) {
                    v.push_back(std::make_pair(i, j));
                }
            }
        }
    }
}
unsigned long Knight::getNextMove(Board &board, bool player, int position) {
    unsigned long nextState = 0;
    Position pieceLocation(position);
    std::vector<std::pair<int, int> > moves = { {1, 2}, {2,1}, {2,-1}, {1, -2}, {-1,-2}, {-2, -1}, {-2,1}, {-1,2} };
    for (auto &move : moves) {
        Position nextPos(pieceLocation.rank + move.first, pieceLocation.file + move.second);
        if (isValidMove(board, player, nextPos)) {
            nextState |= (board.getMask(nextPos.toPosition()));
        }
    }
    return nextState;
}

bool Knight::isValidMove(Board &board, bool color, Position position) {
    if ((position.rank > 7) || (position.file > 7) || (position.rank < 0) || (position.file < 0)) {
        return false;
    }
    if (board.getColor(position.toPosition()) == color) {
        return false;
    }
    return true;

}