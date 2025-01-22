//token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <enums.h>

class Token
{
public:

    int type;
    int lexeme;

    Token() = default;
    ~Token() = default;
    Token(int _type, int _lexeme);

    void print();
};





#endif