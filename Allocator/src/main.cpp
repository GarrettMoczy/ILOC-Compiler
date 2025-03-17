#include "../includes/parser.h"
#include <cstring>
#include <unistd.h>


void printHelpMessage() {
    std::cout << "Usage: 434alloc [OPTIONS] <arguments>\n"
              << "Options:\n"
              << "  -h            Show this help message and exit\n"
              << "  -x <file>     Perform renaming on the input file\n"
              << "  k <file>      Allocate registers (3 ≤ k ≤ 64) for the input file\n";
}

int main(int argc, char* argv[]) {
    std::string fileName;
    int k = -1;
    bool renameMode = false;

    int opt;
    while ((opt = getopt(argc, argv, "hx:")) != -1) {
        switch (opt) {
            case 'h':
                printHelpMessage();
                return 0;
            case 'x':
                renameMode = true;
                fileName = optarg;
                break;
            case '?':
                std::cerr << "ERROR: Invalid option. Use -h for help.\n";
                return 1;
        }
    }

    // If no -x flag was used, check for k <filename> format
    if (!renameMode && optind < argc) {
        k = std::atoi(argv[optind]);
        if (k < 3 || k > 64) {
            std::cerr << "ERROR: k must be between 3 and 64.\n";
            return 1;
        }
        if (optind + 1 < argc) {
            fileName = argv[optind + 1];
        } else {
            std::cerr << "ERROR: Missing filename after k.\n";
            return 1;
        }
    }

    // Check if file name was set
    if (fileName.empty()) {
        std::cerr << "ERROR: No file name provided\n";
        printHelpMessage();
        return 1;
    }

    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open file '" << fileName << "'\n";
        return 1;
    }

    ir* data = new ir();
    Parser parser(file, data);

    bool success = parser.run();

    if (!success) {
        return 0;
    }

    data->rename();

    if (renameMode) {
        data->vrToCode();
    }
    else {
        data->allocate(k);
        data->prToCode();
    }
    delete data;

    return 0;
}