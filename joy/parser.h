/**
* Chomsky type 2 context free grammar parser
* i.e. can be decided by a push down automaton
* Uses a state stack and a map of string operator to lamda function mapping for CF Joy operators
*/
#pragma once

#include <stack>
#include <cassert>

#include "lexer.h"

namespace joy {

	class parser : public lexer {

		using state_stack_t = std::stack<state_t>;

	public:

		parser(joy_stack& stack, io_device& io, std::string path_to_manual);

		//testing 
		// TODO: remove once joy parser implemented
		void parse(std::string line);

	protected:

		bool parse(token_t& token);

	private:

		bool try_context_free(token_t& token);

		/**
		* recursively descend into nested quotes to add a new one
		*/
		void nest_list(joy_stack& stack, size_t depth);

		/**
		* recursively descend into nested lists to add token
		*/
		void nest_token(token_t& token, joy_stack& stack, size_t depth);

		/**
		* recursively descend into nested lists to modify to quote if any cmd added
		*/
		void nest_modify(joy_stack& stack, size_t depth);

		/**
		* error state - unwind the state stack by depth
		*/
		void unwind();

		state_stack_t state_stack;
		size_t list_depth{ 0 };
		size_t set_depth{ 0 };
		bool modify{ false }; //to change a list to a quote if any command added

		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy context free grammar commands into their c++ lambda equivalents only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		dictionary_t context_free_translation {
		{"[", [&]() { 
			state_stack.push(state_t::list);
			nest_list(s, list_depth);
			++list_depth;
		}},
		{"]", [&]() { 
			if (list_depth == 0) {
				error(DAGGNOOPEN, "no matching open angle bracket [");
			}
			else {
				if (modify) {
					nest_modify(s, list_depth - 1);
				}
				--list_depth;
				state_stack.pop();
			}
		}}
		};

	};

}
