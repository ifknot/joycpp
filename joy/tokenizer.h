/**
* Converts an input line string into a list (vector) of tokens <any, type> split by wrapped in quotes then split by whitespace
*/
#pragma once

#include <string>
#include <sstream>
#include <regex>

#include "io_device.h"
#include "error.h"
#include "joy_types.h"
#include "joy_stack.h"


namespace joy {

	
	class tokenizer {
	
	public:
	
		tokenizer(io_device& io);

		joy_stack tokenize(std::string line);

	protected:

		io_device& io;

	private:

		bool commenting{ false };

		/**
		* split out all the open-close quote sections into string tokens 
		*/
		static joy_stack split_strings(joy_stack& tokens);

		/**
		* recursively find and split out sigil enclosed sections into tokens	
		*/
		static void rec_sigil_split(token_t token, joy_stack& tokens, char open_sigil, char close_sigil, joy_t sigil_type);

		/**
		* split out all the white space separated sections into string tokens 
		*/
		static joy_stack split_whitespace(joy_stack& tokens);

		/**
		*  removing any leading spaces and/or trailing single line comment
		*/
		static void trim(std::string& line);
	};

}
