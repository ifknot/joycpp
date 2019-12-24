#include "io_device.h"

#include "colour_codes.h"

namespace joy {

	io_device::io_device() : io_device(std::cin, std::cout) {}
	
	io_device::io_device(std::istream& input_stream, std::ostream& output_stream) :
		input_stream(input_stream),
		output_stream(output_stream),
		colouring_{ false }
	{}

	void io_device::colour(std::string col) {
		ink_colour = col;
	}

	void io_device::colouring(bool b) {
		colouring_ = b;
	}

	bool io_device::colouring() const {
		return colouring_;
	}

	void io_device::ok() {
		output_stream << RESET << BOLDBLACK;
		output_stream << "ok\n";
		ink();
	}

	void io_device::ink() {
		if (colouring()) {
			output_stream << ink_colour;
		}
	}
	void io_device::write_line(std::string line) {
		ink();
		output_stream << line << "\n";
	}

	std::string io_device::read_line() const {
		std::string line;
		getline(input_stream, line);
		return line;
	}
}