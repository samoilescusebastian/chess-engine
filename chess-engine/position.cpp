#include <iostream>
#include "constants.h"
#include "position.h"

Position::Position(int rank, int file)
{
    this -> file = file;
    this -> rank = rank;
}

Position::Position(int position) {
    this -> file = position % 8;
    this -> rank = (position >> 3);
}
Position::Position(std::string position) {
    file = position[0] - 'a';
    rank = position[1] - '1';
}
int Position::toPosition() {
     
    return (rank << 3) + file;
}
std::string Position::toAlphaNumeric() {
    return files.substr(file, 1) + ranks.substr(rank, 1);
}
Position& Position::operator=(const Position& pos) {
    file = pos.file;
    rank = pos.rank;
    
    return *this;
}
Position:: Position(const Position& pos) {
    file = pos.file;
    rank = pos.rank;
}
Position::~Position(){}
