/**
* Chomsky type 2 context free grammar parser
* i.e. can be decided by a push down automaton
* Uses a state stack and a map of string operator to lamda function mapping for CF Joy operators
*/
#pragma once

#include <stack>
#include <algorithm>
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

		void parse(joy_stack& stack);

		void parse(joy_stack&& stack);

		bool parse(token_t& token);

		bool is_parsable(token_t& token);

	private:

		bool try_special(token_t& token);

		bool try_context_free(token_t& token);

		/**
		* recursively descend into nested list to add a new list
		*/
		void nest_list(joy_stack& stack, size_t depth);

		/**
		* recursively descend into nested lists to add token
		*/
		void nest_token(token_t& token, joy_stack& stack, size_t depth);

		/**
		* error state - unwind the state stack by depth
		*/
		void unwind();

		//Joy op C++ implementations

		/**
		* step: A [P] -> ...
		* Sequentially putting members of aggregate A onto stack, executes P for each member of A.
		*/
		joy_stack step(joy_stack& stack);

		/**
		* [a b c]│reverse  →            [c b a]│
		*/
		void reverse(joy_stack& stack);

		/**
		* map: A [P] -> B
		* Executes P on each member of aggregate A, collects results in sametype aggregate B.
		*/
		token_t map(joy_stack& stack);

		/**
		* dip: X [P] -> ... X
		* Saves X, executes P, pushes X back onto stack.
		*/
		void dip(joy_stack& stack);

		/**
		* i: [P] -> ...
		* Executes P. So, [P] i == P.
		*/
		void i(joy_stack& stack);

		//vars

		state_stack_t state_stack;
		size_t list_depth{ 0 };
		size_t set_depth{ 0 };
	
		/**
		* state changing operators
		*/
		dictionary_t special_translation {
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
				--list_depth;
				state_stack.pop();
			}
		}}
		};

		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy context free grammar commands into their c++ lambda equivalents only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		dictionary_t context_free_translation {
		// combinators
		{"map", [&]() { if (has({joy_t::quote_t, joy_t::aggregate_t}, s)) { s.push(map(s)); } }},
		{"reverse", [&]() { if (has({joy_t::aggregate_t}, s)) { reverse(s); } }},
		{"step", [&]() { if (has({joy_t::quote_t, joy_t::aggregate_t}, s)) { parse(step(s)); } }},
		{"dip", [&]() {if (has({joy_t::quote_t, joy_t::any_t}, s)) { dip(s); }}},
		{"i", [&]() { if (has({joy_t::quote_t}, s)) { i(s); } }}
		};

	};

}
