#include "scanner.h"
#include <iostream>

Scanner::Scanner(std::ifstream& _file) : file(_file) {
    initAcceptingStates();
    initCharClasses();
    initTable();
    initTypes();
};

void Scanner::initAcceptingStates() {
    acceptingState[6] = true;
    acceptingState[9] = true;
    acceptingState[14] = true;
    acceptingState[20] = true;
    acceptingState[27] = true;
    acceptingState[28] = true;
    acceptingState[33] = true;
    acceptingState[37] = true;
    acceptingState[44] = true;
    acceptingState[46] = true;
    acceptingState[47] = true;
    acceptingState[49] = true;
    acceptingState[50] = true;
    acceptingState[52] = true;
    acceptingState[56] = true;
    acceptingState[57] = true;
    acceptingState[58] = true;
};

void Scanner::initCharClasses() {
    std::fill(&charClass[0], &charClass[NUM_CHARS], 26); //26 is being used to represent any character not found in ILOC, EOL
    charClass['s'] = 0;
    charClass['u'] = 1;
    charClass['b'] = 2;
    charClass['t'] = 3;
    charClass['o'] = 4;
    charClass['r'] = 5;
    charClass['e'] = 6;
    charClass['l'] = 7;
    charClass['a'] = 8;
    charClass['d'] = 9;
    charClass['I'] = 10;
    charClass['h'] = 11;
    charClass['i'] = 12;
    charClass['f'] = 13;
    charClass['m'] = 14;
    charClass['n'] = 15;
    charClass['p'] = 16;
    charClass['='] = 17;
    charClass['>'] = 18;
    charClass['\n'] = 19;
    charClass[','] = 20;
    charClass[' '] = 21;
    charClass['/'] = 22;
    charClass['\t'] = 23;
    for (char c = '0'; c <= '9'; ++c) charClass[c] = 24;
}

void Scanner::initTable() {
    std::fill(&table[0][0], &table[0][0] + (STATES * TRANSITIONS), 59);
    // trim white space at beginning
    table[0][21] = 0;
    table[0][23] = 0;

    
    // "store "
    table[0][0] = 1; 
    table[1][3] = 2;      
    table[2][4] = 3;      
    table[3][5] = 4;      
    table[4][6] = 5;      
    table[5][21] = 6;    
    
    // "sub "     
    table[1][1] = 7;       
    table[7][2] = 8;       
    table[8][21] = 9;     

    // "load "
    table[0][7] = 10;
    table[10][4] = 11;
    table[11][8] = 12;
    table[12][9] = 13;
    table[13][21] = 14;

    // "loadI "
    table[13][10] = 51;
    table[51][21] = 52;

    // "lshift "
    table[10][0] = 15;
    table[15][11] = 16;
    table[16][12] = 17;
    table[17][13] = 18;
    table[18][3] = 19;
    table[19][21] = 20;

    // "rshift "
    table[0][5] = 21;
    table[21][0] = 22;
    table[22][11] = 23;
    table[23][12] = 24;
    table[24][13] = 25;
    table[25][3] = 26;
    table[26][21] = 27;

    // "r..." 
    table[21][24] = 28;
    table[28][24] = 28;

    // "mult "
    table[0][14] = 29;
    table[29][1] = 30;
    table[30][7] = 31;
    table[31][3] = 32;
    table[32][21] = 33;

    // "add "
    table[0][8] = 34;
    table[34][9] = 35;
    table[35][9] = 36;
    table[36][21] = 37;

    // "output "
    table[0][4] = 38;
    table[38][1] = 39;
    table[39][3] = 40;
    table[40][16] = 41;
    table[41][1] = 42;
    table[42][3] = 43;
    table[43][21] = 44;

    // "=>"
    table[0][17] = 45;
    table[45][18] = 46;

    // ","
    table[0][20] = 47;

    // "//..."  
    table[0][22] = 48;
    table[48][22] = 49;
    std::fill(&table[49][0], &table[50][0],49);
    table[49][25] = 59;
    table[49][19] = 59;

    // "[0-9]+"
    table[0][24] = 50;
    table[50][24] = 50;

    //"nop "
    table[0][15] = 53;
    table[53][4] = 54;
    table[54][16] = 55;
    table[55][21] = 56;   

    //EOF
    table[0][25] = 57;

    //EOL
    table[0][19] = 58;
}

void Scanner::initTypes() {
    std::fill(&type[0],&type[STATES], Token(ERR,ERR));
    type[6] = Token(MEMOP,STORE);
    type[9] = Token(ARITHOP,SUB);
    type[14] = Token(MEMOP,LOAD);
    type[20] = Token(ARITHOP,LSHIFT);
    type[27] = Token(ARITHOP,RSHIFT);
    type[28] = Token(REG,0);
    type[33] = Token(ARITHOP,MULT);
    type[37] = Token(ARITHOP,ADD);
    type[44] = Token(OUTPUT,OUTPUT);
    type[46] = Token(INTO,INTO);
    type[47] = Token(COMMA,COMMA);
    type[49] = Token(COMMENT,COMMENT);
    type[50] = Token(CONSTANT,0);
    type[52] = Token(LOADI,LOADI);
    type[56] = Token(NOP,NOP);
    type[57] = Token(ENDFILE,ENDFILE);
    type[58] = Token(ENDLINE,ENDLINE);
}

void Scanner::clear() {
    char c = file.get();
    while(c != EOF && c != '\n' && c != ' ' && c  != ',' && c != '=') {
        c = file.get();
    }
    file.unget();
}

Token Scanner::nextToken() {
    int state = 0;
    int prevState = 0;
    int lexeme = 0;

    while(state != 59) {
        prevState = state;
        char c = file.get();

        if ('0' <= c && c <= '9') {
            lexeme = lexeme * 10 + (c - '0');
        }

        state = (c == EOF ? table[state][25] : table[state][charClass[c]]);
    }
    if (!acceptingState[prevState]) {
        clear();
        return Token(ERR,ERR);
    }

    file.unget();

    Token token = type[prevState];

    if (token.type == REG || token.type == CONSTANT) {
        token.lexeme = lexeme;
    }

    return token;
}