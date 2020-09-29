#include <utility> 
#include "board.h"
#include "position.h"
#include "queen.h"
#include "constants.h"
#include "rook.h"
#include "bishop.h"

Queen::Queen() {};

Queen::~Queen() {};

void Queen::getAllQueenMoves(Board& board, std::vector<std::pair<int,int>>& allMoves, bool player) {
    unsigned long bitmap = board.getBitMap(QUEEN, player);
    unsigned long result;
    for (int i = 0; i < POSITIONS; i++) {
        if ((bitmap & board.masks[i]) != 0) {
            result = Rook::getNextMoves(board, player, i);
            result |= Bishop::getNextMoves(board, player, i);
            for (int j = 0; j < POSITIONS; j++) {
                if ((result & board.masks[j]) != 0) {
                    allMoves.push_back(std::make_pair(i, j));
                }
            }
        }
    }
}