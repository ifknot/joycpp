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

