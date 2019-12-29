#pragma once

#include <stack>
#include <cassert>

#include "lexer.h"

namespace joy {

	class parser : public lexer {

	public:

		parser(joy_stack& stack0, io_device& io, std::string path_to_manual);

		bool operator()(std::string&& line);

	protected:

		virtual bool tokenize(const std::string& lexeme) override;

	private:

		joy_stack s1; //stack #1 auxillary stack
		std::stack<state_t> state_stack;
		std::string quote_build;
		std::string list_build;
		std::string set_build;
		std::string string_build;

		/**
		* search the context free expression c++ dictionary for a recognised Joy command
		* cascade down to test for states
		* finally display drop down to lexer to parse regular expressions
		*/
		bool try_parse(const std::string& lexeme);

		bool try_context_free(const std::string& lexeme);

		bool try_string(const std::string& lexeme);

		bool try_build_quote(const std::string& lexeme);

		bool try_build_string(const std::string& lexeme);

		virtual void joy_include();
		
		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents
		* only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		cpp_dictionary_t context_free_translation{
		{"include", [&]() { if (expects(1, joy_t::string_t)) { joy_include(); } }},
		{"[", [&]() { state_stack.push(state_t::quote); try_build_quote(); }},
		{"]", [&]() { s0.push(make_quoted_token(quote_build)); state_stack.pop();  }}
		};

	};

}

