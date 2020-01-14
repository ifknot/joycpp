/**
* Error messages
* When a Joy program is processed, two kinds of programming errors can occur. 
*	 1. Occur during reading of an input file. 
*	 2. Runtime when a term is being executed. 
*
* Errors of the first kind occur during reading a program. 
* Since the input line is still available at that time, it is possible for the error message to point to the exact place at which the error occurred. 
* If input comes from the initial input file, most likely the terminal, it is not repeated. 
* In other case it is repeated, including its line number. The next line then consists just of an up-arrow "^" pointing to the error position. 
* Following that is a line containing the specific error message.
*
* All other errors are runtime errors. These are given in the form:
*
*   "run time error: "  MESSAGE  " needed for "  NAME
* e.g.
*      run time error: fewer include files needed for include
*      run time error: valid file name needed for include
*
* In general, the NAME is the name of the operator or combinator whose execution was attempted but failed. 
* The MESSAGE is often the same for many operators or combinators. It can be one of the following:
*/
#pragma once

#include <string>
#include <map>

#include "joy_error_numbers.h"

namespace joy {

	static std::map<size_t, std::string> joy_error_messages = {
		{XNOOPENSIGIL, "opening sigil "},
		{XJOYERROR, "run time error: "},
		{0, "needed for "}
		/*
		{D, "one parameter" },
		{D,"two parameters" },
		{D,"three parameters" },
		{D,"four parameters" },
		{D,"more parameters" },
		{D,"quotation as top parameter" },
		{D,"quotation as second parameter" },
		{D,"quotation as third parameter" },
		{D,"quotation as fourth parameter" },
		{D,"two parameters of the same type" },
		{D,"integer" },
		{D,"two integers" },
		{D,"numeric" },
		{D,"numeric second parameter" },
		{D,"non - zero operand" },
		{D,"list" },
		{D,"list as second parameter" },
		{D,"user defined symbol" },
		{D,"internal list" },
		{D,"small numeric" },
		{D,"non - empty set" },
		{D,"non - empty string" },
		{D,"non - empty list" },
		{D,"smaller index" },
		{D,"aggregate parameter" },
		{D,"different type" }
		*/
	};

}
