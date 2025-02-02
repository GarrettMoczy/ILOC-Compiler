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
    STORE,
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
    SCAN,
    DEFAULT
};

enum OPCODE {
    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_LSHIFT,
    OP_RSHIFT,
    OP_LOAD,
    OP_STORE,
    OP_LOADI,
    OP_OUTPUT,
    OP_NOP
};

#endif // ENUMS_H