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
		std::string string_build;
		size_t quote_depth{ 0 };
		size_t list_depth{ 0 };
		size_t set_depth{ 0 };

		/**
		* test if is a recognized context free command
		*/
		bool is_context_free(const std::string& lexeme);

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
		{"include", [&]() { if (conforms({joy_t::string_t}, s0)) { joy_include(); } }},
		{"[", [&]() { try_build_quote("["); }},
		{"]", [&]() { err(DAGGNOOPEN, "no opening angle bracket ["); }}
		};

	};

}

