#include <iostream>
#include "error.h"
#include "colour_codes.h"

namespace joy {

	bool error(size_t message, std::string name) {
		std::cerr << RESET << RED;
		std::cerr << (joy_error_messages[1] + joy_error_messages[message] + joy_error_messages[0] + name);
		std::cerr << BOLDWHITE << std::endl;
		return false;
	}

}