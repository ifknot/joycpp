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

// [ [ 1 2 3 ] [ 4 5 6 ] ] [ [ dup dup * * ] map ] map

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

		bool is_context_free(token_t& token);

	private:

		/**
		* try an execute token as a state changing operator
		*/
		bool state_change(token_t& token);

		/**
		* try to map token to a context free grammar C++ lamda implementation of a Joy operator
		*/
		bool context_free(token_t& token);

		/**
		* recursively descend into nested list to add a new list
		*/
		void nest_list(joy_stack& stack, size_t depth);

		/**
		* recursively descend into nested lists to add token
		* convert list_t to quote_t if a command is added to a list
		*/
		void nest_token(token_t& token, joy_stack& stack, joy_t& type, size_t depth);

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
		dictionary_t state_change_atoms {
		{"[", [&]() { 
			state_stack.push(state_t::list);
			nest_list(root_stack, list_depth);
			++list_depth;
		}},
		{"]", [&]() { 
			if (list_depth == 0) {
				//error(DAGGNOOPEN, "no matching open angle bracket [");
				run_error(XAGGSIGIL, "]");
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
		dictionary_t context_free_atoms {
		// combinators
		{"map", [&]() { if (require("map", {joy_t::quote_t, joy_t::aggregate_t})) { root_stack.push(map(root_stack)); } }},
		{"reverse", [&]() { if (require("reverse", {joy_t::aggregate_t})) { reverse(root_stack); } }},
		{"step", [&]() { if (require("step", {joy_t::quote_t, joy_t::aggregate_t})) { parse(step(root_stack)); } }},
		{"dip", [&]() {if (require("dip", {joy_t::quote_t, joy_t::any_t})) { dip(root_stack); }}},
		{"i", [&]() { if (require("i", {joy_t::group_t})) { i(root_stack); } }}
		};

	};

}
