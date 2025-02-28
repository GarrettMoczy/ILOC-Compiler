#include "../includes/token.h"

Token::Token(int _type, int _lexeme) : type(_type), lexeme(_lexeme) {};

void Token::print() {
   switch (type)
   {
   case TOKEN_TYPE::MEMOP:
      switch (lexeme) 
      {
      case LOAD:
         std::cout << "< MEMOP, \"load\" > ";
         break;
      case STORE:
         std::cout << "< MEMOP, \"store\" > ";
         break;
      default:
         std::cerr << "Issue printing token. Invalid lexeme ";
         break;
      }
      break;
   case TOKEN_TYPE::LOADI:
      std::cout << "< LOADI, \"loadI\" > ";
      break;
   case TOKEN_TYPE::ARITHOP:
      switch (lexeme)
      {
      case ADD:
         std::cout << "< ARITHOP, \"add\"> ";
         break;
      case SUB:
         std::cout << "< ARITHOP, \"sub\"> ";
         break;
      case MULT:
         std::cout << "< ARITHOP, \"mult\" > ";
         break;
      case LSHIFT:
         std::cout << "< ARITHOP, \"lshift\" > ";
         break;
      case RSHIFT:
         std::cout << "< ARITHOP, \"rshift\" > ";
         break;
      default:
         std::cerr << "Issue printing token. Invalid lexeme ";
         break;
      }
      break;
   case TOKEN_TYPE::OUTPUT:
      std::cout << "< OUTPUT, \"output\"> ";
      break;
   case TOKEN_TYPE::NOP:
      std::cout << "< NOP, \"nop\" > ";
      break;
   case TOKEN_TYPE::COMMA:
      std::cout << "< COMMA, \",\" > ";
      break;
   case TOKEN_TYPE::INTO:
      std::cout << "< INTO, \"=>\" > ";
      break;
   case TOKEN_TYPE::REG:
      std::cout << "< REG," << "\"r" << lexeme << "\" > ";
      break;
   case TOKEN_TYPE::CONSTANT:
      std::cout << "< CONSTANT, \"" << lexeme << "\" > ";
      break;
   case TOKEN_TYPE::COMMENT:
      std::cout << "<COMMENT,COMMENT> ";
      break;
   case TOKEN_TYPE::ENDFILE:
      std::cout << "< EOF , \"\" >\n";
      break;
   case TOKEN_TYPE::ENDLINE:
      std::cout << "< EOL, \"\\n\" >\n";
      break;
   case TOKEN_TYPE::ERR:
      std::cout << "<ERR, ERR> ";
      break;
   default:
      break;
   }
};


int Token::OpCode() {
   switch (type)
   {
   case TOKEN_TYPE::MEMOP:
      switch (lexeme) 
      {
      case MEMOP_TYPE::LOAD:
         return OPCODE::OP_LOAD;
         break;
      case MEMOP_TYPE::STORE:
         return OPCODE::OP_STORE;
         break;
      default:
         return -1;
         break;
      }
      break;
   case TOKEN_TYPE::LOADI:
      return OPCODE::OP_LOADI;
      break;
   case TOKEN_TYPE::ARITHOP:
      switch (lexeme)
      {
      case ARITHOP_TYPE::ADD:
         return OPCODE::OP_ADD;
         break;
      case ARITHOP_TYPE::SUB:
         return OPCODE::OP_SUB;
         break;
      case ARITHOP_TYPE::MULT:
         return OPCODE::OP_MULT;
         break;
      case ARITHOP_TYPE::LSHIFT:
         return OPCODE::OP_LSHIFT;
         break;
      case  ARITHOP_TYPE::RSHIFT:
         return OPCODE::OP_RSHIFT;
         break;
      default:
         return -1;
         break;
      }
      break;
   case TOKEN_TYPE::OUTPUT:
      return OPCODE::OP_OUTPUT;
      break;
   case TOKEN_TYPE::NOP:
      return OPCODE::OP_NOP;
      break;
   default:
      return -1;
      break;
   }
};
