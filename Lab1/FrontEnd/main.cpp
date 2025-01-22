#include "scanner.h"
#include <iostream>

void prettyPrint(std::pair<int,int> token) {
   switch (token.first)
   {
   case  MEMOP:
      switch (token.second) 
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
      switch (token.second)
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
      std::cout << "<OUTPUT, OUTPUT>";
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
      std::cout << "<REG," << token.second << "> ";
      break;
   case CONSTANT:
      std::cout << "<CONSTANT, " << token.second << "> ";
      break;
   case COMMENT:
      std::cout << "<COMMENT,COMMENT> ";
      break;
   case ENDFILE:
      std::cout << "<EOF,EOF>";
      break;
   case ENDLINE:
      std::cout << "<EOL,EOL>\n";
      break;
   case ERR:
      std::cout << "<ERR, ERR>";
      break;
   default:
      std::cout << "<SRS ERR, SRS ERR>";
      break;
   }
};

// const char * helpMessage = "C++ based parser for a simplified version of the ILOC Language\n"
//             "     -h              produces a list of valid command-line arguments\n"
//             "     -r <name>       reads the specified file and prints the intermediate representation\n"
//             "     -p <name>       reads the specified file and reports a success or reports all errors found in the input file\n"
//             "     -s <name>       reads the specified file and prints a list of the tokens found by the scanner\n";

//    const char * missingNameMessage = "Invalid Usage: please include a valid ILOC file name\n";


 int main (int argc, char * argv[]) {
   

    // fflush(stdout);
    // if (argc == 1) {

    // }
    // if (argc == 2) {
    //     if (strcmp(argv[1],"-h") == 0) {
    //         fwrite(helpMessage, sizeof(char), strlen(helpMessage), stdout);
    //     }
    //     else if (strcmp(argv[1],"-r") == 0) {
    //         fwrite(missingNameMessage, sizeof(char), strlen(missingNameMessage), stdout);
    //     }
    //     else if (strcmp(argv[1],"-p") == 0) {
    //         fwrite(missingNameMessage, sizeof(char), strlen(missingNameMessage), stdout);
    //     }
    //     else if (strcmp(argv[1],"-s") == 0) {
    //         fwrite(missingNameMessage, sizeof(char), strlen(missingNameMessage), stdout);
    //     }
    //     else {
    //         //code for default behavior 
    //     }
    // }
    fstream file("../Tests/test_inputs/t4.i"); 
    Scanner s (file);
    pair<int,int> currToken = s.nextToken();
    prettyPrint(currToken);
    while (currToken.first != ENDFILE)
    {
      currToken = s.nextToken();
      prettyPrint(currToken);
    }
    
    return 0;
 }