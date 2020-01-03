/**
* @brief Joy Language Interpreter
* 2020.01.03 refactor (std:any)
*/
#include <iostream>

//use catch2 unit testing for BDD
#define CATCH_CONFIG_RUNNER
#include "catch2.h"

//#define NDEBUG

#ifdef NDEBUG


#else

//chance to try out Behaviour Driven Development
int main(int argc, char* argv[]) {

    std::cout << "\ttest joy" << std::endl;

    Catch::Session().run(argc, argv);

    system("pause");
}

#endif

