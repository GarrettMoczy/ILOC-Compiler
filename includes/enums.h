// enums.h
#ifndef ENUMS_H
#define ENUMS_H

enum TOKENS {
    MEMOP,
    LOADI,
    ARITHOP,
    OUTPUT,
    NOP,
    COMMA,
    INTO,
    REG,
    CONSTANT,
    COMMENT,
    ENDFILE,
    ENDLINE,
    ERR
};

enum MEMOPS {
    LOAD,
    STORE
};

enum ARITHOPS {
    ADD,
    SUB,
    MULT,
    LSHIFT,
    RSHIFT
};

enum MODES {
    HELP,
    REP,
    PARSE,
    SCAN
};

#endif // ENUMS_H