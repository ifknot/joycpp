#pragma once

#include <string>
#include <sstream>
#include <regex>
#include <stack>

#include "io_device.h"
#include "error.h"
#include "joy_types.h"
#include "joy_stack.h"
#include "joy_overloads.h"


namespace joy {

	/**
	* The tokenizer class is the first layer of the joycpp interpreter providing:
	* + conversion input line string into a Joy stack (vector) of tokens <any, type> 
	* + tokenize Joy reserved characters [ ] { } ; . (note: the . reserved character is handled as a special case)
	* + recursively split wrapped in quotes " " and assign string_t (these will need converting to a stack of characters later)
	* + split remainder into tokens delineated by whitespace and assigning any_t 
	* + strip any Joy comments either single line # delineated or multi-line (* *) delineated as per Joy language
	* + special case tokenize the . reserved character to avoid floating point mis-tokenizing 
	* + tokenize the simple types: boolean, char, integer, double
	* + convert the string_t from a std::string into a stack of characters
	*
	* joycpp uses an OOP hierarchy to add layers of functionality:
	* + tokenizer class
	* + regular grammar parser Joy primitives to core C++ implementations
	* + context free grammar parser Joy primitives to core C++ implementations
	* + context free grammar parser Joy to Joy primitives
	*
	* The Joy language can be concatenate built from a very minimal set of Joy primitives expressed in C++.
	* joycpp extends that minimal set of C++ primitives where it makes sense from a performance and OS perspective.
	*
	*/
	class tokenizer {

	protected:
	
		/**
		* If input does not come from the keyboard but from an include file, it is often useful to see the current input on the screen. 
		* Normally the Joy system does not echo keyboard or other input on the screen or output file, but it can be made to do so. 
		* Joy has an internal echo flag with values 0, 1, 2 or 3
		* The initial default value is 0, and then no echo occurs. 
		* If the value is 1, then input lines are echoed to the output without change.
		* If the value is 2, then each line is preceded by a tab (ASCII 9) -- this can help to distinguish echoed lines in the output from others. 
		* If the value is 3, then each tab is preceded by a line number specific to the current input file. 
		* When input reverts to an earlier file, the earlier line numbering is resumed. 
		* The value of the echo flag can be set by the operator setecho. 
		* Typically the value will be set by something like:
		*
        *	2  setecho.
		*/
		enum class echo_state_t {	none,	/// (default) nothing is echoed 
									echo,	/// input lines are echoed
									tab,	/// preceded by a tab (ASCII 9)
									linenumber	/// each tab is preceded by a line number
								};
	
	public:
	
		tokenizer(io_device& io);

		joy_stack tokenize(std::string& line);

		joy_stack tokenize(std::string&& line);
		
	protected:

		size_t line_number{ 0 };
		std::stack<size_t> line_number_stack;

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
