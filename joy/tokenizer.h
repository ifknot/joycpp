#pragma once

#include <string>
#include <sstream>
#include <regex>

#include "io_device.h"
#include "error.h"
#include "joy_types.h"
#include "joy_stack.h"
#include "joy_overloads.h"


namespace joy {

	/**
	* Converts an input line string into a list (vector) of tokens <any, type> split by wrapped in quotes then split by whitespace
	*/
	class tokenizer {
	
	public:
	
		tokenizer(io_device& io);

		//TODO: joy_stack tokenize(std::string& line);

		/**
		* convert user input line into joy_stack of a single undefined token {line, undef_t}
		*/
		joy_stack tokenize(std::string& line);

		joy_stack tokenize(std::string&& line);
		
	protected:

		enum class echo_state_t { none, echo, tab, linenumber };

		//TODO: fix nested line numbers
		size_t line_number{ 0 };

		echo_state_t echo_state{ echo_state_t::none };

		io_device& io;

		virtual joy_stack tokenize(joy_stack& tokens);

		//virtual joy_stack tokenize(joy_stack&& tokens);

		void echo(size_t line_number, const std::string& line);

	private:

		bool comment_block{ false };

		std::vector<char> reserved = { '[', ']', '{', '}', ';' };//, '.'}; 

		/**
		* split out all the open-close quote sections into {std::sting, string_t} tokens 
		*/
		static joy_stack tokenize_strings(joy_stack&& tokens);	

		/**
		* An input line consists of a number of tokens. 
		* Some of these are single characters, the reserved characters. 
		* They are the two (square) brackets [ and ] , the two (curly) braces { and }, the semicolon ; and the period . :
		*
        *      "["  |  "]"  |  "{"  |  "}"  |  ";"  |  "."
		*/
		joy_stack tokenize_reserved(joy_stack&& tokens);

		/**
		* special case handling for "." reserved character to enable correct decimal point recognition
		* for any lexeme over 2 chars in size exclude '. and then test and split an trailing .
		*/
		joy_stack tokenize_period(joy_stack&& tokens);

		/**
		* recursively find and split reserved character and assign type
		*/
		static void rec_char_split(token_t token, joy_stack& tokens, char ch, joy_t char_type);

		/**
		* recursively find and split out sigil enclosed sections into tokens
		*/
		static void rec_sigil_split(token_t token, joy_stack& tokens, std::string open_sigil, std::string close_sigil, joy_t sigil_type);

		/**
		* comments are not further processed but are treated just like blank space.
		*/
		joy_stack strip_comments(joy_stack&& tokens);

		/**
		* split out all the white space separated sections into string tokens 
		*/
		static joy_stack split_whitespace(joy_stack&& tokens);

		/**
		* tokenize bool_t, char_t, int_t, double_t
		* convert string_t to a aggregate joy_stack of char_t tokens
		*/
		static joy_stack tokenize_simple_types(joy_stack&& tokens);

		/**
		* regex "^ +"
		*/
		inline void trim_leading_whitespace(std::string& line) {
			line = std::regex_replace(line, std::regex("^ +"), "");
		}

		/**
		* comments are not further processed but are treated just like blank space.
		*/
		inline void trim_trailing_comment(std::string& line) {
			line = line.substr(0, line.find_first_of('#'));
		}
	};

}
