#pragma once

#include <stack>

#include "lexer.h"

namespace joy {

	class parser : public lexer {

		enum class state_t { regular_s, quote_s, list_s, set_s, string_s, define_s };

	public:

		using lexer::lexer;

	protected:

		virtual bool tokenize(const std::string& lexeme) override;

		/**
		* search the context free expression c++ dictionary for a recognised Joy command
		* cascade down to test for states
		* finally display drop down to lexer to parse regular expressions
		*/
		bool try_parse(const std::string& lexeme);

	private:

		joy_stack s1; //stack #1 auxillary stack
		std::stack<state_t> state_stack;

		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents
		* only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		//cpp_dictionary_t context_free_translation{};

	};

}

