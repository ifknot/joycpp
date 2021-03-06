/**
* Joy Language Interpreter
* 2020.01.03 re-factor (std:any)
*/
#include <iostream>

#include "io_device.h"
#include "joy_stack.h"
#include "parse_joy.h"

void run() { // exec loop

    joy::io_device io;
    io.colouring(true);
    joy::joy_stack stack;
    
    joy::parse_joy parser(stack, io, "joy_manuals/joy03.man");
    while (!parser.is_quit()) {

        if (parser.root_parse(parser.tokenize(io.readline()))) {
            io.ok();
            io.colour(parser.state_to_colour(parser.state()));
            io << parser.state_to_string(parser.state());
        }
        else {
            io.ok();
            io.colour(parser.state_to_colour(parser.state()));
            io << parser.state_to_string(parser.state());
        }
        
    }

}

/**
* 4 usage options:
* Interactive command line interpreter
* File input - command line output
* Command line input - file output
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

    system("pause");

}
