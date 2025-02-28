// scanner.h
#ifndef SCANNER_H
#define SCANNER_H

#include <fstream>
#include "token.h"
#include "buffer.h"
#include "enums.h"

using std::pair;

constexpr int STATES = 60; // 59 is error state
constexpr int TRANSITIONS = 27; // all valid ILOC characters plus one transition for all other characters
constexpr int NUM_CHARS = 256; //ascii characters

class Scanner
{
private:

    Buffer file;

    int table[STATES][TRANSITIONS];
    bool acceptingState[STATES]{};
    int charClass[NUM_CHARS];
    Token type[STATES];

    void initTable();
    void initAcceptingStates();
    void initCharClasses();
    void initTypes();

    void clear();

public:
    Scanner(std::ifstream& _file);
    ~Scanner() = default;
    Token nextToken();
};

#endif