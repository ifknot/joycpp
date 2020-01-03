#pragma once

#include <string>
#include <map>

#include "error_numbers.h"

namespace joy {

	static const std::string ERR = "error #";

	static std::map<error_number_t, std::string> debug_messages = {
	{DJOYERROR, "runtime exception"},
	{DSTACKEMPTY, "empty stack"},
	{DNOCONVERSION, "no conversion"},
	{DFILENOTFOUND, "file not found"},
	{DMANNOTFOUND, "not found in manual"},
	{DLESSARGS, "not enough arguments on stack"},
	{DAGGNOOPEN, "aggregate error"},
	{DQUOTESET, "no lists or quotes permitted in sets"},
	{DPRECISION, "possible loss of precision"},
	{DWRONGTYPE, "wrong type"}
	};

}