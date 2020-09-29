#include <iostream>
#include <fstream>
#include <utility> 
#include "board.h"
#include "position.h"
#include "king.h"
#include "constants.h"

King::King() {};

King::~King() {};
void King::getAllKingMoves(Board& board, std::vector<std::pair<int,int>>& allMoves, bool player) {
    unsigned long bitmap = board.getBitMap(KING, player);
    unsigned long result;
    for (int i = 0; i < POSITIONS; i++) {
        if ((bitmap & board.masks[i]) != 0) {
            result = getNextMoves(board, player, i);
            for (int j = 0; j < POSITIONS; j++) {
                if ((result & board.masks[j]) != 0) {
                    allMoves.push_back(std::make_pair(i, j));
                }
            }
        }
    }
}

unsigned long King::getNextMoves(Board& board, bool player, int position) {
    unsigned long result = 0;
    Position pos(position);
    int currentPosition;

    for (int i = 0; i < 8; ++i) {
        currentPosition = position + kingValues[i];
        if (pos.file == 7 && (kingValues[i] == 1 || kingValues[i] == 9 || kingValues[i] == -7)) {
            continue;
        }
        if (pos.file == 0 && (kingValues[i] == -1 || kingValues[i] == -9 || kingValues[i] == 7)) {
            continue;
        }
        // Check if the next move is still on the table
        if (currentPosition < 0 || currentPosition > 63) {
            continue;
        }

        // Check if the next square is available
        if (board.isOccupied(currentPosition) && 
            board.getBitMapAtPosition(currentPosition, player)) {
            continue;
        }

        // Check if the next move is allowed
        if (isKingAttacked(board, player, currentPosition)) {
            continue;
        }
        
        result |= board.getMask(currentPosition);
    }
    checkCastle(board, position, result, player);
    return result;
}

bool King::isKingAttacked(Board& board, bool player, int position) {
    Position pos(position);

    // Check for pawns
    int piece;
    int currentPosition;
       if (player == WHITE) {
        piece = board.getPieceAtPosition(position + 7, !player);
        if (piece == PAWN) {
            return true;
        }
        piece = board.getPieceAtPosition(position + 9, !player);
        if (piece == PAWN) {
            return true;
        }
    } else {
        piece = board.getPieceAtPosition(position - 7, !player);
        if (piece == PAWN) {
                return true;
        }
        piece = board.getPieceAtPosition(position - 9, !player);
        if (piece == PAWN) {
            return true;
        }
    }

    for (int i = 0; i < 8; i++) {
        currentPosition = position + kingValues[i];
         if (pos.file == 7 && (kingValues[i] == 1 || kingValues[i] == 9 || kingValues[i] == -7)) {
            continue;
        }
        if (pos.file == 0 && (kingValues[i] == -1 || kingValues[i] == -9 || kingValues[i] == 7)) {
            continue;
        }
        // Check if the next move is still on the table
        if (currentPosition < 0 || currentPosition > 63) {
            continue;
        }
        if (board.getPieceAtPosition(currentPosition, !player) == KING) {
            return true;
        }
    }

    // Check for knights
    for (int i = 0; i < 8; ++i) {
        currentPosition = position + knightValues[i];
        if (currentPosition < 0 || currentPosition > 63) {
            continue;
        }
        if (board.getPieceAtPosition(currentPosition, !player) == KNIGHT) {
            return true;
        }
    }

    // Check for bishops and queen(diagonal)
    currentPosition = pos.toPosition() - 9; 
    for (int i = pos.file - 1; i >= 0 && currentPosition >= 0; --i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player) != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == BISHOP || piece == QUEEN) {
                return true;
            }
            break;
        }

        currentPosition -= 9; 
    }
    
    currentPosition = pos.toPosition() + 9; 
    for (int i = pos.file + 1; i < 8 && currentPosition < 64; ++i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == BISHOP || piece == QUEEN) {
                return true;
            }
            break;
        }

        currentPosition += 9; 
    }

    currentPosition = pos.toPosition() + 7;
    for (int i = pos.file - 1; i >= 0 && currentPosition < 64; --i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == BISHOP || piece == QUEEN) {
                return true;
            }
            break;
        }

        currentPosition += 7; 
    }

    currentPosition = pos.toPosition() - 7;
    for (int i = pos.file + 1; i < 8 && currentPosition >= 0; ++i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == BISHOP || piece == QUEEN) {
                return true;
            }
            break;
        }

        currentPosition -= 7; 
    }

    // Check for rooks and queen(horizontal and vertical)
    currentPosition = pos.toPosition() - 8; 
    for (int i = pos.rank - 1; i >= 0; --i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == ROOK || piece == QUEEN) {
                return true;
            }
            break;
        }

        currentPosition -= 8; 
    }
    
    currentPosition = pos.toPosition() + 8; 
    for (int i = pos.rank + 1; i < 8; ++i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == ROOK || piece == QUEEN) {
                return true;
            }
            break;
        }

        currentPosition += 8; 
    }

    currentPosition = pos.toPosition() - 1;
    for (int i = pos.file - 1; i >= 0; --i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == ROOK || piece == QUEEN) {
                return true;
            }
            break;
        }

        currentPosition -= 1; 
    }

    currentPosition = pos.toPosition() + 1;
    for (int i = pos.file + 1; i < 8; ++i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == ROOK || piece == QUEEN) {
                return true;
            }
            break;
        }

        currentPosition += 1; 
    }

    return false;
}

void King::checkCastle(Board& board, int position, unsigned long &result, bool color) {
    if (board.kingsMoved[color] || (board.castle[color][0] && board.castle[color][1])) {
        return;
    }
    if (King::isKingAttacked(board, color, position)) {
        return;
    }
    Position pos(position);
    int currentPosition = position + 1;
    for (int i = 0; i < (8 - pos.file) - 2; i++) {
        if (board.isOccupied(currentPosition) || King::isKingAttacked(board, color, currentPosition))  {
            break;
        }
        currentPosition++;
    }
    Position pos1(currentPosition);
    if (pos1.file == 7 && board.getPieceAtPosition(currentPosition, color) == ROOK && !board.castle[color][1]) {
        result |= board.getMask(position + 2);
    }

    currentPosition = position - 1;
    for (int i = pos.file-2; i >= 0; i--) {
        if (board.isOccupied(currentPosition) || King::isKingAttacked(board, color, currentPosition)) {
            break;
        }
        currentPosition--;
    }
    Position pos2(currentPosition);
    if (pos2.file == 0 && board.getPieceAtPosition(currentPosition, color) == ROOK && !board.castle[color][0]) {
        result |= board.getMask(position - 2);
    }
}

bool King::moveInCheck(Board &board, bool player, int position, std::unordered_set<int> &mySet) {
    Position pos(position);
    int piece;
    int currentPosition;
    bool check = false;
    if (player == WHITE) {
    piece = board.getPieceAtPosition(position + 7, !player);
    if (piece == PAWN) {
        check = true;
    }
    piece = board.getPieceAtPosition(position + 9, !player);
    if (piece == PAWN) {
        check = true;
    }
} else {
    piece = board.getPieceAtPosition(position - 7, !player);
    if (piece == PAWN) {
        check = true;
    }
    piece = board.getPieceAtPosition(position - 9, !player);
    if (piece == PAWN) {
        check = true;
    }
}

    for (int i = 0; i < 8; i++) {
        currentPosition = position + kingValues[i];
         if (pos.file == 7 && (kingValues[i] == 1 || kingValues[i] == 9 || kingValues[i] == -7)) {
            continue;
        }
        if (pos.file == 0 && (kingValues[i] == -1 || kingValues[i] == -9 || kingValues[i] == 7)) {
            continue;
        }
        // Check if the next move is still on the table
        if (currentPosition < 0 || currentPosition > 63) {
            continue;
        }
        if (board.getPieceAtPosition(currentPosition, !player) == KING) {
            check = true;
        }
    }

    // Check for knights
    for (int i = 0; i < 8; ++i) {
        currentPosition = position + knightValues[i];
        if (currentPosition < 0 || currentPosition > 63) {
            continue;
        }
        if (board.getPieceAtPosition(currentPosition, !player) == KNIGHT) {
            check = true;
        }
    }

    // Check for pawns


    currentPosition = pos.toPosition() - 9; 
    for (int i = pos.file - 1; i >= 0 && currentPosition >= 0; --i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player) != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == BISHOP || piece == QUEEN) {
                check = true;
            }
            piece = board.getPieceAtPosition(currentPosition, player);
            if (piece != UNDEFINED) {
                mySet.insert(currentPosition);
            }
            break;
        }

        currentPosition -= 9; 
    }
    
    currentPosition = pos.toPosition() + 9; 
    for (int i = pos.file + 1; i < 8 && currentPosition < 64; ++i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == BISHOP || piece == QUEEN) {
                check = true;
            }
            piece = board.getPieceAtPosition(currentPosition, player);
            if (piece != UNDEFINED) {
                mySet.insert(currentPosition);
            }
            break;
        }

        currentPosition += 9; 
    }

    currentPosition = pos.toPosition() + 7;
    for (int i = pos.file - 1; i >= 0 && currentPosition < 64; --i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == BISHOP || piece == QUEEN) {
                check = true;
            }
            piece = board.getPieceAtPosition(currentPosition, player);
            if (piece != UNDEFINED) {
                mySet.insert(currentPosition);
            }
            break;
        }

        currentPosition += 7; 
    }

    currentPosition = pos.toPosition() - 7;
    for (int i = pos.file + 1; i < 8 && currentPosition >= 0; ++i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == BISHOP || piece == QUEEN) {
                check = true;
            }
            piece = board.getPieceAtPosition(currentPosition, player);
            if (piece != UNDEFINED) {
                mySet.insert(currentPosition);
            }
            break;
        }

        currentPosition -= 7; 
    }

    // Check for rooks and queen(horizontal and vertical)
    currentPosition = pos.toPosition() - 8; 
    for (int i = pos.rank - 1; i >= 0; --i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == ROOK || piece == QUEEN) {
                check = true;
            }
            piece = board.getPieceAtPosition(currentPosition, player);
            if (piece != UNDEFINED) {
                mySet.insert(currentPosition);
            }
            break;
        }

        currentPosition -= 8; 
    }
    
    currentPosition = pos.toPosition() + 8; 
    for (int i = pos.rank + 1; i < 8; ++i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == ROOK || piece == QUEEN) {
                check = true;
            }
            piece = board.getPieceAtPosition(currentPosition, player);
            if (piece != UNDEFINED) {
                mySet.insert(currentPosition);
            }
            break;
        }

        currentPosition += 8; 
    }

    currentPosition = pos.toPosition() - 1;
    for (int i = pos.file - 1; i >= 0; --i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == ROOK || piece == QUEEN) {
                check = true;
            }
            piece = board.getPieceAtPosition(currentPosition, player);
            if (piece != UNDEFINED) {
                mySet.insert(currentPosition);
            }
            break;
        }

        currentPosition -= 1; 
    }

    currentPosition = pos.toPosition() + 1;
    for (int i = pos.file + 1; i < 8; ++i) {
        if (board.isOccupied(currentPosition) && board.getPieceAtPosition(currentPosition, player)  != KING) {
            piece = board.getPieceAtPosition(currentPosition, !player);
            if (piece == ROOK || piece == QUEEN) {
                check = true;
            }
            piece = board.getPieceAtPosition(currentPosition, player);
            if (piece != UNDEFINED) {
                mySet.insert(currentPosition);
            }
            break;
        }

        currentPosition += 1; 
    }

    return check;

}
