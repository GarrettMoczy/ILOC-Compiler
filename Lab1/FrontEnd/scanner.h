// scanner.h
#ifndef SCANNER_H
#define SCANNER_H

#include "common_includes.h"


using std::pair, std::fstream;

constexpr int STATES = 60; // 59 is error state
constexpr int TRANSITIONS = 27; // all valid ILOC characters plus one transition for all other characters
constexpr int NUM_CHARS = 256; //ascii characters
constexpr int BUF_SIZE = 1024; //change to test performance

class Scanner
{
private:
    fstream& file;
    int table[STATES][TRANSITIONS];
    bool acceptingState[STATES]{};
    int charClass[NUM_CHARS];
    pair<int,int> type[STATES];
    char buf[BUF_SIZE];
    int bufPos = 0;
    int toRead = 0;
    void initAcceptingStates();
    void initCharClasses();
    void initTable();
    void initType();
    void clear();
public:
    Scanner(fstream& _file);
    ~Scanner() = default;
    pair<int, int> nextToken();
};

#endif