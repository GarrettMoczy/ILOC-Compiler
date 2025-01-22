// enums.h
#ifndef ENUMS_H
#define ENUMS_H

enum TOKEN_TYPE {
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

enum MEMOP_TYPE {
    LOAD,
    STORE
};

enum ARITHOP_TYPE {
    ADD,
    SUB,
    MULT,
    LSHIFT,
    RSHIFT
};

enum PARSER_MODE {
    HELP,
    REP,
    PARSE,
    SCAN
};

#endif // ENUMS_H