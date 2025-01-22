#include "token.h"

Token::Token(int _type, int _lexeme) : type(_type), lexeme(_lexeme) {};

void Token::print() {
   switch (type)
   {
   case  MEMOP:
      switch (lexeme) 
      {
      case LOAD:
         std::cout << "<MEMOP,LOAD> ";
         break;
      case STORE:
         std::cout << "<MEMOP,STORE> ";
         break;
      default:
         std::cout << "<MEMOP,ERR> ";
         break;
      }
      break;
   case LOADI:
      std::cout << "<LOADI,LOADI> ";
      break;
   case ARITHOP:
      switch (lexeme)
      {
      case ADD:
         std::cout << "<ARITHOP,ADD> ";
         break;
      case SUB:
         std::cout << "<ARITHOP,SUB> ";
         break;
      case MULT:
         std::cout << "<ARITHOP,MULT> ";
         break;
      case LSHIFT:
         std::cout << "<ARITHOP,LSHIFT> ";
         break;
      case RSHIFT:
         std::cout << "<ARITHOP,RSHIFT> ";
         break;
      default:
         std::cout << "<ARITHOP,ERR> ";
         break;
      }
      break;
   case OUTPUT:
      std::cout << "<OUTPUT, OUTPUT> ";
      break;
   case NOP:
      std::cout << "<NOP,NOP> ";
      break;
   case COMMA:
      std::cout << "<COMMA,COMMA> ";
      break;
   case INTO:
      std::cout << "<INTO,INTO> ";
      break;
   case REG:
      std::cout << "<REG," << lexeme << "> ";
      break;
   case CONSTANT:
      std::cout << "<CONSTANT, " << lexeme << "> ";
      break;
   case COMMENT:
      std::cout << "<COMMENT,COMMENT> ";
      break;
   case ENDFILE:
      std::cout << "<EOF,EOF> ";
      break;
   case ENDLINE:
      std::cout << "<EOL,EOL>\n";
      break;
   case ERR:
      std::cout << "<ERR, ERR> ";
      break;
   default:
      break;
   }
};


