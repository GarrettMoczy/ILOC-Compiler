#include "parser.h"
#include <cstring>

const char * helpMessage = 
  "C++ based parser for a simplified version of the ILOC language.\n\n"
  "Usage:\n"
  "      ./434fe [flags] filename\n\n"
  "Required arguments:\n"
  "   filename    the pathname (absolute or relative) to the input file\n\n"
  "The following optional flags are mutually exclusive and listed in order of descending priority:\n"
  "      -h       prints this message.\n"
  "      -r       prints the IR constructed by the parser.\n"
  "      -p       reports success or errors found.\n"
  "      -s       prints a list of tokens found by the scanner.\n"
  "If no flag is specificed the default behavior is -p\n";


PARSER_MODE parseArguments(int argc, char* argv[], std::string& fileName) {
    PARSER_MODE mode = DEFAULT;
    bool fileProvided = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" && mode > HELP) {
            mode = HELP;
        } else if (arg == "-r" && mode > REP) {
            mode = REP;
        } else if (arg == "-p" && mode > PARSE) {
            mode = PARSE;
        } else if (arg == "-s" && mode > SCAN) {
            mode = SCAN;
        } else if (!fileProvided) {
            fileName = arg;
            fileProvided = true;
        } else {
            std::cerr << "ERROR: Multiple file names provided\n";
            std::cout << helpMessage;
            exit(EXIT_FAILURE);
        }
    }

    if (mode == DEFAULT) {
        mode = PARSE;
    }

    return mode;
}

int main(int argc, char* argv[]) {
    std::string fileName;
    PARSER_MODE mode = parseArguments(argc, argv, fileName);

    if (mode == HELP) {
        std::cout << helpMessage;
        return 0;
    }

    if (fileName.empty()) {
        std::cerr << "ERROR: No file name provided\n";
        std::cout << helpMessage;
        return 0;
    }

  std::ifstream file(fileName);
  if (!file.is_open()) {
    std::cerr << "ERROR: Could not open file";
    return 0;
  }

  ir * data = new ir();

  Parser parser (file, mode, data);

  parser.run();

  delete data;

  return 0;
}