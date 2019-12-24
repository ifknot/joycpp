/**
* @brief Joy Language Interpreter
* Two part main: NDEBUG switches between running the interpreter and testing its parts 
*/
//#define NDEBUG

#include <iostream>

//use catch2 unit testing for BDD
#define CATCH_CONFIG_RUNNER
#include "catch2.h"

#ifdef NDEBUG

/**
* 4 usage options:
* Interactive command line interpreter
* File input - command line output
* Command line - file output
* File input - file output
*/
int main(int argc, char* argv[]) {

    std::cout << "*** Joy ***" << std::endl;

    switch (argc){

    default:
        break;
    }

    system("pause");
}

#else

//chance to try out Behaviour Driven Development
int main(int argc, char* argv[]) {

    std::cout << "\ttest joy" << std::endl;

    Catch::Session().run(argc, argv);

    system("pause");
}

#endif

