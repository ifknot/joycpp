#pragma once

#include <string>
#include <map>

#include "error_numbers.h"

namespace joy {

	static const std::string ERR = "error #";

	static std::map<error_number_t, std::string> debug_messages = {
	{DNOCONVERSION, "unrecognised input"},
	{DWRONGTYPE, "wrong type expected:"}
	};

}