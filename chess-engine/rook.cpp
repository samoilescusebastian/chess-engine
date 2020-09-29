#include <iostream>
#include <fstream>
#include <utility> 
#include "board.h"
#include "position.h"
#include "rook.h"
#include "constants.h"

Rook::Rook() {};

Rook::~Rook() {};
void Rook::getAllRookMoves(Board& board, std::vector<std::pair<int,int>>& allMoves, bool player) {
    unsigned long bitmap = board.getBitMap(ROOK, player);
    unsigned long result;
    for (int i = 0; i < POSITIONS; i++) {
        if ((bitmap & board.masks[i]) != 0) {
            result = Rook::getNextMoves(board, player, i);
            for (int j = 0; j < POSITIONS; j++) {
                if ((result & board.masks[j]) != 0) {
                    allMoves.push_back(std::make_pair(i, j));
                }
            }
        }
    }
}

unsigned long Rook::getNextMoves(Board& board, bool player, int position) {
    unsigned long result = 0;
    Position pos(position);
    int currentPosition;

    currentPosition = pos.toPosition() - 8; 
    for (int i = pos.rank - 1; i >= 0; --i) {
        if (board.isOccupied(currentPosition)) {
            if (board.getBitMapAtPosition(currentPosition, !player)) {
                result |= board.getMask(currentPosition);
            }
            break;
        }

        result |= board.getMask(currentPosition);

        currentPosition -= 8; 
    }
    
    currentPosition = pos.toPosition() + 8; 
    for (int i = pos.rank + 1; i < 8; ++i) {
        if (board.isOccupied(currentPosition)) {
            if (board.getBitMapAtPosition(currentPosition, !player)) {
                result |= board.getMask(currentPosition);
            }
            break;
        }

        result |= board.getMask(currentPosition);

        currentPosition += 8; 
    }

    currentPosition = pos.toPosition() - 1;
    for (int i = pos.file - 1; i >= 0; --i) {
        if (board.isOccupied(currentPosition)) {
            if (board.getBitMapAtPosition(currentPosition, !player)) {
                result |= board.getMask(currentPosition);
            }
            break;
        }

        result |= board.getMask(currentPosition);

        currentPosition -= 1; 
    }

    currentPosition = pos.toPosition() + 1;
    for (int i = pos.file + 1; i < 8; ++i) {
        if (board.isOccupied(currentPosition)) {
            if (board.getBitMapAtPosition(currentPosition, !player)) {
                result |= board.getMask(currentPosition);
            }
            break;
        }

        result |= board.getMask(currentPosition);

        currentPosition += 1; 
    }

    return result;
}