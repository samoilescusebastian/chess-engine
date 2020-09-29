#include <iostream>

#ifndef _POSITION_H_
#define _POSITION_H_
class Position
{
    private:
    
    public:
        int file;
        int rank;

        Position(int, int);
        Position(int);
        Position(std::string);
        ~Position();
        Position(const Position&);
        Position& operator=(const Position&);
        int toPosition();
        std::string toAlphaNumeric();
};
#endif // !_POSITION_H_
