#include "../includes/parser.h"

Parser::Parser(std::ifstream& file, ir *_data) : line(1), typeErr(0), syntaxErr(0), maxSR(-1), scan(file)  {
    data = _data;
};

void Parser::finish_memop() {

    int arg1, arg3;
    int opcode = currToken.OpCode();
    currToken = scan.nextToken();

    if (currToken.type != REG && currToken.type) {
        std::cerr << "ERROR " << line << ": REG token expected after MEMOP ";
        syntaxErr++;
        handle_err();
        return;
    }
    arg1 = currToken.lexeme;
    
    data->updateSR(arg1);

    currToken = scan.nextToken();

    if (currToken.type != INTO) {
        std::cerr << "ERROR " << line << ": INTO token expected after REG  ";
        syntaxErr++;
        handle_err();
        return;
    }

    currToken = scan.nextToken();
    
    if (currToken.type != REG) {
        std::cerr << "ERROR " << line << ": REG token expected after INTO ";
        syntaxErr++;
        handle_err();
        return;
    }

    arg3 = currToken.lexeme;

    data->updateSR(arg3);

    currToken = scan.nextToken();

    if (currToken.type == COMMENT) currToken = scan.nextToken();
    
    if (currToken.type != ENDFILE && currToken.type != ENDLINE) {
        std::cerr << "ERROR " << line << ": EOL/EOF token expected after REG ";
        syntaxErr++;
        handle_err();
        return;
    }
    if (opcode == OPCODE::OP_LOAD)
        data->emplace_back(line, opcode, arg1,-1,arg3);
    else
        data->emplace_back(line,opcode,arg1,arg3,-1);

    line++;
}

void Parser::finish_loadI() {
    int arg1, arg3;
    int opcode = currToken.OpCode();

    currToken = scan.nextToken();

    if (currToken.type != CONSTANT) {
        std::cerr << "ERROR " << line << ": CONSTANT token expected after LOADI ";
        syntaxErr++;
        handle_err();
        return;
    }

    arg1 = currToken.lexeme;

    data->updateSR(arg1);

    currToken = scan.nextToken();

    if (currToken.type != INTO) {
        std::cerr << "ERROR " << line << ": INTO token expected after CONSTANT  ";
        syntaxErr++;
        handle_err();
        return;
    }
    currToken = scan.nextToken();

    if (currToken.type != REG) {
        std::cerr << "ERROR " << line << ": REG token expected after INTO ";
        syntaxErr++;
        handle_err();
        return;
    }

    arg3 = currToken.lexeme;

    data->updateSR(arg3);

    currToken = scan.nextToken();

    if (currToken.type == COMMENT) currToken = scan.nextToken();
    

    if (currToken.type != ENDFILE && currToken.type != ENDLINE) {
        std::cerr << "ERROR " << line << ": EOL/EOF token expected after REG ";
        syntaxErr++;
        handle_err();
        return;
    }

    data->emplace_back(line, opcode, arg1,-1,arg3);

    line++;
}

void Parser::finish_arithop() {
    int arg1, arg2, arg3;
    int opcode = currToken.OpCode();

    currToken = scan.nextToken();

    if (currToken.type != REG) {
        std::cerr << "ERROR " << line << ": REG token expected after ARITHOP ";
        syntaxErr++;
        handle_err();
        return;
    }

    arg1 = currToken.lexeme;

    data->updateSR(arg1);

    currToken = scan.nextToken();

    if (currToken.type != COMMA) {
        std::cerr << "ERROR " << line << ": COMMA token expected after first REG  ";
        syntaxErr++;
        handle_err();
        return;
    }

    currToken = scan.nextToken();

    if (currToken.type != REG) {
        std::cerr << "ERROR " << line << ": second REG token expected after COMMA ";
        syntaxErr++;
        handle_err();
        return;
    }

    arg2 = currToken.lexeme;

    data->updateSR(arg2);

    currToken = scan.nextToken();

    if (currToken.type != INTO) {
        std::cerr << "ERROR " << line << ": INTO token expected after second REG ";
        syntaxErr++;
        handle_err();
        return;
    }
    
    currToken = scan.nextToken();

    if (currToken.type != REG) {
        std::cerr << "ERROR " << line << ": dest REG token expected after INTO ";
        syntaxErr++;
        handle_err();
        return;
    }
    
    arg3 = currToken.lexeme;

    data->updateSR(arg3);

    currToken = scan.nextToken();

    if (currToken.type == COMMENT) currToken = scan.nextToken();
    
    if (currToken.type != ENDFILE && currToken.type != ENDLINE) {
        std::cerr << "ERROR " << line << ": EOL/EOF token expected after REG ";
        syntaxErr++;
        handle_err();
        return;
    }

    data->emplace_back(line, opcode, arg1,arg2,arg3);

    line++;
}


void Parser::finish_output() {
    int arg1;
    int opcode = currToken.OpCode();

    currToken = scan.nextToken();

    if (currToken.type != CONSTANT) {
        std::cerr << "ERROR " << line << ": CONSTANT token expected after OUTPUT ";
        syntaxErr++;
        handle_err();
        return;
    }

    arg1 = currToken.lexeme;
    
    currToken = scan.nextToken();

    if (currToken.type == COMMENT) currToken = scan.nextToken();

    if (currToken.type != ENDFILE && currToken.type != ENDLINE) {
        std::cerr << "ERROR " << line << ": EOL/EOF token expected after CONSTANT ";
        syntaxErr++;
        handle_err();
        return;
    }

    data->emplace_back(line, opcode, arg1,-1,-1);

    line++;
}

void Parser::finish_nop() {
    // int opcode = currToken.OpCode();

    currToken = scan.nextToken();

    if (currToken.type == COMMENT) currToken = scan.nextToken();
    
    if (currToken.type != ENDFILE && currToken.type != ENDLINE) {
        std::cerr << "ERROR " << line << ": EOL/EOF token expected after CONSTANT ";
        syntaxErr++;
        handle_err();
        return;
    }
    // data->emplace_back(line, opcode, -1,-1,-1);

    line++;
}

void Parser::handle_err() {
    while(currToken.type != ENDLINE && currToken.type != ENDFILE) {
        if (currToken.type == ERR) {
            typeErr++;
        }
        currToken = scan.nextToken();
    }
    std::cout << '\n';
    line++;
}


void Parser::parse()
{
    while (currToken.type != ENDFILE)
    {
        switch (currToken.type)
        {
        case MEMOP:
            finish_memop();
            break;
        case LOADI:
            finish_loadI();
            break;
        case ARITHOP:
            finish_arithop();
            break;
        case OUTPUT:
            finish_output();
            break;
        case NOP:
            finish_nop();
            break;
        case COMMENT:
            scan.nextToken();
            line++;
            break;
        case ENDLINE:
            line++;
            break;
        default:
            std::cerr << "ERROR " << line <<  ": Invalid or unknown operation ";
            handle_err();
            syntaxErr++;
            break;
        }
        currToken = scan.nextToken();
    }
}



bool Parser::run() {
    currToken = scan.nextToken();
   
    parse();
    if (typeErr + syntaxErr == 0) {
        // std::cout << "File successfully parsed with " << data->size() <<  " operations.\n";
        return true;
    }
    std::cerr << "Parsing failed with " << typeErr << " type and " << syntaxErr << " syntax error/s.\n";
    return false;
}
