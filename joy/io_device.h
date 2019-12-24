/**
 * IO Devices access
 * Interface between an ascii input stream (e.g. std::cin or a file) 
 * and a, possibly colourised, ascii output stream (e.g. std::cout or a file)
 */
#pragma once

#include <iostream>
#include <fstream>
#include <string>

namespace joy {

    class io_device {

    public:

        io_device();

        io_device(std::istream& input_stream, std::ostream& output_stream);

        void colour(std::string col);

        void colouring(bool b);

        bool colouring() const;

        void ok();

    private:

        void ink();

        void write_line(std::string line);

        std::string read_line() const;

        std::string ink_colour;
        std::istream& input_stream;
        std::ostream& output_stream;
        bool colouring_;

        friend io_device& operator<<(io_device& d, std::string line) {
            d.ink();
            d.output_stream << line << "\n";
            return d;
        }

        friend io_device& operator>>(io_device& d, std::string& line) {
            d.input_stream >> line;
            return d;
        }

    };

}

