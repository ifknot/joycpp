#pragma once

#include <string>
#include <map>

#include "error_numbers.h"

namespace joy {

	static const std::string ERR = "error #";

	static std::map<error_number_t, std::string> error_messages = {
	{DLESSARGS, "insufficent arguments"},
	{DWRONGTYPE, "type mismatch"},
	{DNOCONVERSION, "no conversion"},
	{DJOYERROR, "runtime exception"}
	};

}