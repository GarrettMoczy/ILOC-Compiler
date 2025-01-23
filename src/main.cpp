#include "scanner.h"
#include <iostream>
#include <chrono>


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
    std::ifstream file("/mnt/class_files/Classes/CSCE-434/Class/lab1/test_inputs/T128k.i"); 
    
    if (!file.is_open()) {
      std::cout << "Err opening file";
      return 0;
    }
    Scanner s (file);

    auto start = std::chrono::high_resolution_clock::now();

    Token currToken = s.nextToken();
   // currToken.print();


   while (currToken.type != ENDFILE)
    {
      currToken = s.nextToken();
     // currToken.print();
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time to scan: " << duration.count() << " microseconds\n";
    return 0;
 }