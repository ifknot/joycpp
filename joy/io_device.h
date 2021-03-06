#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "colour_codes.h"

namespace joy {

    /**
     * IO Devices access
     * Interface between an ASCII input stream (e.g. std::cin or a file)
     * and a, possibly colourised, ASCII output stream (e.g. std::cout or a file)
     */
    class io_device {

    public:

        /**
        * default to std::cin and std::cout 
        */
        io_device();

        io_device(std::istream& input_stream, std::ostream& output_stream);

        std::string readline();

        /**
        * set the text colour to an ANSI escape code for terminal colours
        */
        void colour(std::string col);

        /**
        * switch ANSI terminal colouring on and off
        */
        void colouring(bool b);

        bool colouring() const;

        /**
        * send light gray "ok" message and CR to the output stream
        */
        void ok(std::string info = "");

        /**
        * send characters to output stream
        */
        void put_string(std::string s);


        /**
        * get an input from the input stream
        */
        std::string input();

    private:

        std::string ink_colour{ BOLDWHITE };
        std::istream& input_stream;
        std::ostream& output_stream;
        bool colouring_{ false };

        /**
        * inject ANSI terminal colour codes in the output stream if colouring enabled
        */
        void ink();

        /**
        * maintain C++ idioms for output
        */
        friend io_device& operator<<(io_device& d, std::string line);

    };

}

