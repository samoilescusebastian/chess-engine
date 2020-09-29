#include <iostream>
#include <fstream>
#include <utility> 
#include "board.h"
#include "position.h"
#include "bishop.h"
#include "constants.h"

Bishop::Bishop() {};

Bishop::~Bishop() {};
void Bishop::getAllBishopMoves(Board& board, std::vector<std::pair<int,int>>& allMoves, bool player) {
    unsigned long  bitmap = board.getBitMap(BISHOP, player);
    unsigned long  result;
    for (int i = 0; i < POSITIONS; i++) {
        if ((bitmap & board.masks[i]) != 0) {
            result = Bishop::getNextMoves(board, player, i);
            for (int j = 0; j < POSITIONS; j++) {
                if ((result & board.masks[j]) != 0) {
                    allMoves.push_back(std::make_pair(i, j));
                }
            }
        }
    }
}
unsigned long  Bishop::getNextMoves(Board& board, bool player, int position) {
    unsigned long  bitmap = board.getMask(position);
    unsigned long  result = 0;
    int file = position % 8;

    int currentPosition = position + 9;
    for (int i = 1; (i < (8 - file)) && (currentPosition < 64); i++) {
        if (board.isOccupied(currentPosition)) {
            if (board.getBitMapAtPosition(currentPosition, !player)) {
                result |= (bitmap << 9 * i);
            }
            break;
        }
        result |= (bitmap << 9 * i);
        currentPosition += 9;
    }
    
    currentPosition = position + 7;
    for (int i = 1; (i < file  + 1) && (currentPosition < 64); i++) {
        if (board.isOccupied(currentPosition)) {
            if (board.getBitMapAtPosition(currentPosition, !player)) {
                result |= (bitmap << 7 * i);
            }
            break;
        }
        result |= (bitmap << 7 * i);
        currentPosition += 7;
    
    }

    currentPosition = position - 7;
    for (int i = 1; (i < 8 - file) && (currentPosition >= 0); i++) {
        if (board.isOccupied(currentPosition)) {
            if (board.getBitMapAtPosition(currentPosition, !player)) {
                result |= (bitmap >> 7 * i);
            }
            break;
        }
        result |= (bitmap >> 7 * i);
        currentPosition -= 7;
    
    }

    currentPosition = position - 9;
    for (int i = 1; (i < file  + 1) && (currentPosition >= 0); i++) {
        if (board.isOccupied(currentPosition)) {
            if (board.getBitMapAtPosition(currentPosition, !player)) {
                result |= (bitmap >> 9 * i);
            }
            break;
        }
        result |= (bitmap >> (9 * i));
        currentPosition -= 9;
    
    }

    return result;
}