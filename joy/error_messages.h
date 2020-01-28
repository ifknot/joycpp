#pragma once

#include <string>
#include <map>

#include "error_numbers.h"

namespace joy {

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
	static std::map<size_t, std::string> joy_error_messages = {
		{XRESERVED, "reserved word usage "},
		{XSYNTAX, "correct syntax "},
		{XDEFNREJECTED, "definition rejected valid Joy syntax "},
		{XNOFILE, "valid file "},
		{XAGGSIGIL, "opening bracket "},
		{XARGC, "more arguments "},
		{XTYPEMISMATCH, "correct type "},
		{XNOCONVERSION, "Joy03 syntax "},
		{XNOOPENSIGIL, "opening sigil "},
		{XJOYERROR, "run time error: "},
		{0, "needed for "}
	};

}
