#include "io_device.h"

namespace joy {

	io_device::io_device() : io_device(std::cin, std::cout) {}

	io_device::io_device(std::istream& input_stream, std::ostream& output_stream) :
		input_stream(input_stream),
		output_stream(output_stream)
	{}

	std::string io_device::readline() {
		std::string line;
		getline(input_stream, line);
		return line;
	}

	void io_device::colour(std::string col) {
		ink_colour = col;
	}

	void io_device::colouring(bool b) {
		colouring_ = b;
	}

	bool io_device::colouring() const {
		return colouring_;
	}

	void io_device::ok(std::string info) {
		output_stream << RESET << BOLDBLACK;
		output_stream << "ok " << info;
	}

	void io_device::ink() {
		if (colouring()) {
			output_stream << RESET << ink_colour;
		}
	}

	io_device& operator<<(io_device& d, std::string line) {
		d.ink();
		d.output_stream << line << "\n";
		return d;
	}

}