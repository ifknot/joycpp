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

		/**
		* cascade the lexeme down the context free conversion steps:
		* context free, string, quote, list, set
		* then onto lexer::tokenize
		*/
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

		//parsing cascade

		/**
		* examine the top of the state stack and adopt state:
		* parse - try_context_free then string, then the lexeer cascade
		* quote - build a quote
		* list - build a list
		* set - build a set
		* string - build a string
		*/
		bool try_parse(const std::string& lexeme);

		/**
		* try and convert lexeme to a context free Joy command
		* by searching the rcontext free c++ dictionary
		* return true on success
		* return try_string if fail
		*/
		bool try_context_free(const std::string& lexeme);

		/**
		* try an and convert lexeme to a string - string is a special case needing its own try 
		* return true on success
		* return lexer::try_regular if fail
		*/
		bool try_string(const std::string& lexeme);

		/**
		* build a quote until close quote
		*/
		bool try_build_quote(const std::string& lexeme);

		/**
		* build a string until close string
		*/
		bool try_build_string(const std::string& lexeme);

		//Joy command translations: (where in map lambdas would be too long)

		virtual void include();
		
		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents
		* only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		cpp_dictionary_t context_free_translation{
		{"include", [&]() { if (conforms({joy_t::string_t}, s0)) { include(); } }},
		{"[", [&]() { try_build_quote("["); }},
		{"]", [&]() { error(DAGGNOOPEN, "no opening angle bracket ["); }}
		};

	};

}

