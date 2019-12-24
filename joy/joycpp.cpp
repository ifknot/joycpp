/**
* @brief Joy Language Interpreter
* Two part main: NDEBUG switches between running the interpreter and testing its parts 
*/
#include <iostream>

//use catch2 unit testing for BDD
#define CATCH_CONFIG_RUNNER
#include "catch2.h"

#define NDEBUG

#ifdef NDEBUG

#include <iostream>
#include <fstream>
#include <stdio.h>

void run() { 
    // exec loop
    std::cout << "run loop\n";
}

/**
* 4 usage options:
* Interactive command line interpreter
* File input - command line output
* Command line - file output
* File input - file output
*/
int main(int argc, char* argv[]) {

    std::cout << "*** Joy ***\n";

    switch (argc) {
    case 1: {
        run();
        break;
    }
    case 3: {
        if (strcmp(argv[1], "-i") == 0) {
            std::ifstream input_file(argv[2]);
            if (!input_file) {
                std::cout << "no such file " << argv[2];
            }
            else {
                run();
            }
        }
        else if (strcmp(argv[1], "-o") == 0) {
            std::ofstream output_file(argv[2]);
            if (!output_file) {
                std::cout << "could not open " << argv[2];
            }
            else {
                run();
            }
        }
        else {
            std::cout << "usage: joy [ -i input_file ] [-o output_file ]" << std::endl;
            std::exit(-1);
        }
        break;
        }
    case 5: {
        if ((strcmp(argv[1], "-i") == 0) && (strcmp(argv[3], "-o") == 0)) {
            std::ifstream input_file(argv[2]);
            std::ofstream output_file(argv[4]);
            run();
        }
        else {
            std::cout << "usage: joy [ -i input_file ] [-o output_file ]" << std::endl;
            std::exit(-1);
        }
        break;
    }
    default:
        std::cout << "usage: joy [ -i input_file ] [-o output_file ]" << std::endl;
        std::exit(-1);
        break;
    }

    std::cout << "fin" << std::endl;

    system("pause");

}

#else

#include "joy_stack.h"

using namespace joy;

//chance to try out Behaviour Driven Development
int main(int argc, char* argv[]) {

    std::cout << "\ttest joy" << std::endl;

    joy_stack j;

    auto t = std::make_pair("1", joy_t::int_t);

    j.push(t);
    j.dump();

    Catch::Session().run(argc, argv);

    system("pause");
}

#endif

