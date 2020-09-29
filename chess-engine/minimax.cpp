#include "minimax.h"
#include "constants.h"
void restoreParameters(Board& board, int oldKingPos, int oldEnPassantPos, bool kingsMoved, bool castles[2], int oldNoPieces, bool player) {
    board.kingsMoved[player] = kingsMoved;
    board.castle[player][0] = castles[0];
    board.castle[player][1] = castles[1];
    board.kingPositions[player] = oldKingPos;
    board.enPassant = oldEnPassantPos;
    board.noPieces = oldNoPieces;
}
void undoMove(Board& board, std::vector<std::tuple<int, int, bool>>& setSet, std::vector<std::tuple<int, int, bool>>& unsetSet,
              std::unordered_map<unsigned long, int>& freq, std::string encoding) {
    freq[board.hashZobrist]--;
    int position, piece;
    bool color;
    for (auto &tuple : setSet) {
        position = std::get<0>(tuple);
        piece = std::get<1>(tuple);
        color = std::get<2>(tuple);
        board.setPosition(position, piece, color);
        board.hashZobrist ^= board.randomZobrist[color][piece][position];
    }
    for (auto &tuple : unsetSet) {
        position = std::get<0>(tuple);
        piece = std::get<1>(tuple);
        color = std::get<2>(tuple);
        board.unsetPosition(position, piece, color);
        board.hashZobrist ^= board.randomZobrist[color][piece][position];
    }
    setSet.clear();
    unsetSet.clear();
}
int getScore(Board& board, bool player) {
    int score = 0; 
    std::vector<int> ourPawns(8, 0);
    std::vector<int> enemyPawns(8, 0);
    int enemyBishopCount = 0, ourBishopCount = 0;
    for (int i = 0; i < PIECES; i++) {
        for (int j = 0; j < POSITIONS; j++) {
            if ((board.bitboard[i][player] & board.masks[j]) != 0) {
                score += SCORES[i];
                if (i == KING) {
                    if (board.noPieces <= 10) {
                        score += kingEnd[j];
                    }
                    break;
                }
                score += BONUSES[i][player][j];
                if (i == PAWN) {
                    ourPawns[j % 8]++;
                    if (ourPawns[j % 8] > 1) score -= 16;
                }
                if (i == KNIGHT) {
                    if (board.noPieces <= 10) {
                        score -= 10;
                    }
                }
                if (i == BISHOP) {
                    ourBishopCount++;
                }
            } else if((board.bitboard[i][!player] & board.masks[j]) != 0) {
                score -= SCORES[i];
                if (i == KING) {
                    if (board.noPieces <= 10) {
                        score -= kingEnd[63 - j];
                    }
                    break;
                }
                score -= BONUSES[i][!player][j];
                if (i == PAWN) {
                    enemyPawns[j % 8]++;
                    if (enemyPawns[j % 8] > 1) score += 16;
                }
                if (i == KNIGHT) {
                    if (board.noPieces <= 10) {
                        score += 10;
                    }
                }
                if (i == BISHOP) {
                    enemyBishopCount++;

                }                
            }
        }
    }

    // enemy isolated pawns
    if (enemyPawns[0] >= 1 && enemyPawns[1] == 0) {
        score += 12;
    }
    if (enemyPawns[1] >= 1 && enemyPawns[0] == 0 && enemyPawns[2] == 0) {
        score += 12;
    }
    if (enemyPawns[2] >= 1 && enemyPawns[1] == 0 && enemyPawns[3] == 0) {
        score += 12;
    }
    if (enemyPawns[3] >= 1 && enemyPawns[2] == 0 && enemyPawns[4] == 0) {
        score += 12;
    }
    if (enemyPawns[4] >= 1 && enemyPawns[3] == 0 && enemyPawns[5] == 0) {
        score += 12;
    }
    if (enemyPawns[5] >= 1 && enemyPawns[4] == 0 && enemyPawns[6] == 0) {
        score += 12;
    }
    if (enemyPawns[6] >= 1 && enemyPawns[5] == 0 && enemyPawns[7] == 0) {
        score += 12;
    }
    if (enemyPawns[7] >= 1 && enemyPawns[6] == 0) {
        score += 12;
    }

    // our isolated pawns
    if (ourPawns[0] >= 1 && ourPawns[1] == 0) {
        score -= 12;
    }
    if (ourPawns[1] >= 1 && ourPawns[0] == 0 && ourPawns[2] == 0) {
        score -= 14;
    }
    if (ourPawns[2] >= 1 && ourPawns[1] == 0 && ourPawns[3] == 0) {
        score -= 16;
    }
    if (ourPawns[3] >= 1 && ourPawns[2] == 0 && ourPawns[4] == 0) {
        score -= 20;
    }
    if (ourPawns[4] >= 1 && ourPawns[3] == 0 && ourPawns[5] == 0) {
        score -= 20;
    }
    if (ourPawns[5] >= 1 && ourPawns[4] == 0 && ourPawns[6] == 0) {
        score -= 16;
    }
    if (ourPawns[6] >= 1 && ourPawns[5] == 0 && ourPawns[7] == 0) {
        score -= 14;
    }
    if (ourPawns[7] >= 1 && ourPawns[6] == 0) {
        score -= 12;
    }

    // Black passed pawns
    if(enemyPawns[0] >= 1 && ourPawns[0] == 0) {
        score -= enemyPawns[0];
    }
    if(enemyPawns[1] >= 1 && ourPawns[1] == 0) {
        score -= enemyPawns[1];
    }
    if(enemyPawns[2] >= 1 && ourPawns[2] == 0) {
        score -= enemyPawns[2];
    }
    if(enemyPawns[3] >= 1 && ourPawns[3] == 0) {
        score -= enemyPawns[3];
    }
    if(enemyPawns[4] >= 1 && ourPawns[4] == 0) {
        score -= enemyPawns[4];
    }
    if(enemyPawns[5] >= 1 && ourPawns[5] == 0) {
        score -= enemyPawns[5];
    }
    if(enemyPawns[7] >= 1 && ourPawns[6] == 0) {
        score -= enemyPawns[6];
    }
    if(enemyPawns[7] >= 1 && ourPawns[7] == 0) {
        score -= enemyPawns[7];
    }

    // White passed pawns
    if(ourPawns[0] >= 1 && enemyPawns[0] == 0) {
        score += ourPawns[0];
    }
    if(ourPawns[1] >= 1 && enemyPawns[1] == 0) {
        score += ourPawns[1];
    }
    if(ourPawns[2] >= 1 && enemyPawns[2] == 0) {
        score += ourPawns[2];
    }
    if(ourPawns[3] >= 1 && enemyPawns[3] == 0) {
        score += ourPawns[3];
    }
    if(ourPawns[4] >= 1 && enemyPawns[4] == 0) {
        score += ourPawns[4];
    }
    if(ourPawns[5] >= 1 && enemyPawns[5] == 0) {
        score += ourPawns[5];
    }
    if(ourPawns[7] >= 1 && enemyPawns[6] == 0) {
        score += ourPawns[6];
    }
    if(ourPawns[7] >= 1 && enemyPawns[7] == 0) {
        score += ourPawns[7];
    }

    if (board.noPieces <= 10) {
        if (ourBishopCount >= 2) {
            score += 10;
        }
        if (enemyBishopCount >= 2) {
            score -= 10;
        }
    }
    return score;
}
void sortMoves(Board &board, std::vector<std::pair<int,int>> &moves, bool color, std::ofstream &miniFile) {
    std::unordered_map<unsigned long, int> freq;
    int oldKingPos, oldEnPassantPos, oldNoPieces;
    bool kingsMoved, castles[2];
    struct Score {
        int from;
        int to; 
        int score;
        bool operator< (const Score& move2) {
            return score > move2.score;
        }
    };
    std::vector<std::tuple<int, int, bool>> setSet;
    std::vector<std::tuple<int, int, bool>> unsetSet;
    std::string s = "plm";
    std::vector<Score> scores;
    Score temp;
    kingsMoved = board.kingsMoved[color];
    oldKingPos = board.kingPositions[color];
    castles[0] = board.castle[color][0];
    castles[1] = board.castle[color][1];
    oldEnPassantPos = board.enPassant;
    oldNoPieces = board.noPieces;
    for (auto &move : moves) {
        temp.from = move.first;
        temp.to = move.second;
        board.updateBoard(temp.from, temp.to, color, setSet, unsetSet, freq, s);
        temp.score = getScore(board, color);
        scores.push_back(temp);
        undoMove(board, setSet, unsetSet, freq, s);
        restoreParameters(board, oldKingPos, oldEnPassantPos, kingsMoved, castles, oldNoPieces, color);
    }
    int n = scores.size();
    std::sort(scores.begin(), scores.end());
    for (int i = 0; i < n; i++) {
        moves[i].first = scores[i].from;
        moves[i].second = scores[i].to;
    }
}
void getPossibleMoves(Board& board, std::vector<std::pair<int,int>> &moves, bool color, std::ofstream& miniFile) {
    moves.clear();
    King::getAllKingMoves(board, moves, color);
    Pawn::getAllPawnMoves(board, moves, color, miniFile);
    Rook::getAllRookMoves(board, moves, color);
    Knight::getAllKnightMoves(board, moves, color);
    Bishop::getAllBishopMoves(board, moves, color);
    Queen::getAllQueenMoves(board, moves, color);
    sortMoves(board, moves, color, miniFile);
}


struct Value {
    std::pair<Move, int> move;
    int depth;
};   
std::unordered_map<unsigned long, Value> transposition[2];
std::pair<Move, int> minimax(Board& board, bool player, int depth, int alpha, int beta,
                     std::unordered_map<unsigned long, int>& freq, std::ofstream& miniFIle) {
    
    if (freq[board.hashZobrist] == 3) {
        miniFIle << "egal\n";
        miniFIle.flush();
        int score = getScore(board, player);
        return std::make_pair(Move(2, 2), -score);
    }
    if (depth == 0) {
        int score = getScore(board, player);
        return std::make_pair(Move(3, 3), score);
    }
    std::unordered_set<int> chessPieces;
    bool check = King::moveInCheck(board, player, board.kingPositions[player], chessPieces);
    bool checkMate = true;
    std::pair<Move, int> bestMove;
    std::vector<std::pair<int,int>> nextMoves;
    std::vector<std::tuple<int, int, bool>> setSet;
    std::vector<std::tuple<int, int, bool>> unsetSet;
    std::pair<Move, int> bestLocalMove;
    std::string encoding;
    int oldKingPos, oldEnPassantPos, oldNoPieces;
    bool kingsMoved, castles[2];
    getPossibleMoves(board, nextMoves, player, miniFIle);

    bestMove.second = MIN_VALUE;

    for (unsigned int j = 0; j < nextMoves.size(); j++) {
        std::pair<int, int> currentMove = nextMoves[j];
        kingsMoved = board.kingsMoved[player];
        oldKingPos = board.kingPositions[player];
        castles[0] = board.castle[player][0];
        castles[1] = board.castle[player][1];
        oldEnPassantPos = board.enPassant;
        oldNoPieces = board.noPieces;
        board.updateBoard(currentMove.first, currentMove.second, player, setSet, unsetSet, freq, encoding);         
        
        if (check || chessPieces.find(currentMove.first) != chessPieces.end()) {
            bestLocalMove.first.from = currentMove.first;
            bestLocalMove.first.to = currentMove.second;
            if (King::isKingAttacked(board, player, board.kingPositions[player])) {
                undoMove(board, setSet, unsetSet, freq, encoding);
                restoreParameters(board, oldKingPos, oldEnPassantPos, kingsMoved, castles, oldNoPieces, player);
                continue;
            }
        }
        checkMate = false;
        bestLocalMove = minimax(board, !player, depth - 1, -beta, -alpha, freq, miniFIle);
        bestLocalMove.second = -bestLocalMove.second;
        bestLocalMove.first.from = currentMove.first;
        bestLocalMove.first.to = currentMove.second;
        undoMove(board, setSet, unsetSet, freq, encoding);  
        restoreParameters(board, oldKingPos, oldEnPassantPos, kingsMoved, castles, oldNoPieces, player);
        if (bestLocalMove.second == MAX_VALUE) {
            return bestLocalMove;
        }
        if (bestMove.second <= bestLocalMove.second) {
            bestMove = bestLocalMove;
        }
        if (bestMove.second > alpha) {
            alpha = bestMove.second;
        }
        if (alpha > beta) {
            break;
        }
    }
    if (checkMate) {
        if (check) {
            return std::make_pair(Move(63, 63), MIN_VALUE);
        } else {
            return std::make_pair(Move(1, 1), MAX_VALUE);
        }
    }
    return bestMove;

}