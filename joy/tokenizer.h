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

		/**
		* convert user input line into joy_stack of a single undefined token {line, undef_t}
		*/
		joy_stack tokenize(std::string&& line);
		
	protected:

		io_device& io;

		/**
		* split the line into recognised tokens bool_t, char_t, int_t, double_t, string_t
		* mark any remainder as undef_t
		*/
		virtual joy_stack tokenize(joy_stack&& tokens);


	private:

		/**
		* split out all the open-close quote sections into {std::sting, string_t} tokens 
		*/
		static joy_stack tokenize_strings(joy_stack&& tokens);	

		static joy_stack tokenize_comments(joy_stack&& tokens);

		/**
		* filter out comment tokens
		*/
		static joy_stack strip_comments(joy_stack&& tokens);

		/**
		* recursively find and split out sigil enclosed sections into tokens	
		*/
		static void rec_sigil_split(token_t token, joy_stack& tokens, std::string open_sigil, std::string close_sigil, joy_t sigil_type);

		/**
		* split out all the white space separated sections into string tokens 
		*/
		static joy_stack split_whitespace(joy_stack&& tokens);

		/**
		* tokenize bool_t, char_t, int_t, double_t
		* convert string_t to a aggregate joy_stack of char_t tokens
		*/
		static joy_stack tokenize_simple_types(joy_stack&& tokens);

		inline void trim_leading_whitespace(std::string& line) {
			line = std::regex_replace(line, std::regex("^ +"), "");
		}

		inline void trim_trailing_comment(std::string& line) {
			line = line.substr(0, line.find_first_of('#'));
		}
	};

}
