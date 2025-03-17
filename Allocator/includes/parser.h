//parser.h
#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "ir.h"

class Parser 
{
private:
    int mode, line, typeErr, syntaxErr, maxSR;
    Scanner scan;
    Token currToken;
    ir *data;
    void finish_memop();
    void finish_loadI();
    void finish_arithop();
    void finish_output();
    void finish_nop();
    void handle_err();
    void parse();
public:
    Parser(std::ifstream &file, ir* data);
    ~Parser() = default;
    bool run();
};




#endif