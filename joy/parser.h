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

#include "joy_minimal.h"
#include "joy_combinators.h"

// 2 3 4 [ + ] dip
// [ 1 2 3 ] [ dup * ] step
// [ 1 2 3 4 ] [ + * ] infra
// [ [ 1 2 3 ] [ 4 5 6 ] ] [ [ dup dup * * ] map ] map

namespace joy {

	class parser : public lexer {

		using state_stack_t = std::stack<state_t>;

	public:

		parser(joy_stack& stack, io_device& io, std::string path_to_manual);

		state_t state() const;

		using lexer::tokenize;

		virtual joy_stack tokenize(joy_stack&& tokens) override;

		virtual bool parse(joy_stack& tokens);

		//virtual void no_conversion(joy_stack& tokens);

		/**
		* endow anonymous functor behaviour on parser
		*/
		inline void operator()(joy_stack& P, joy_stack& S) {
			parse(P, S);
		}

		inline void operator()(joy_stack&& P, joy_stack& S) {
			parse(P, S);
		}

	protected:

		void parse(joy_stack& P, joy_stack& S);

		void parse(joy_stack&& P, joy_stack& S);

		bool parse(token_t& token, joy_stack& S);

		bool parse(token_t&& token, joy_stack& S);

		bool is_state_change(token_t& token);

		bool is_context_free(token_t& token);

		bool parse_undef(token_t& token, joy_stack& S);

		bool parse_defined(token_t& token, joy_stack& S);

		inline bool commenting() {
			return state_stack.top() == state_t::comment;
		}

		inline bool parsing() {
			return state_stack.top() == state_t::parse;
		}

		/**
		* push down automata context free stack
		*/
		state_stack_t state_stack;

	private:

		/**
		* tokenize context free expression Joy command types
		*/
		joy_stack tokenize_context_free_types(joy_stack&& tokens);

		/**
		* operator matching function and execute if match return true otherwise return false
		*/
		bool run(token_t& token, joy_stack& S);
		
		size_t list_depth{ 0 };

		/**
		* try ti execute token as a state changing operator
		*/
		bool state_change(token_t& token, joy_stack& S);

		/**
		* try to map token to a context free grammar C++ lamda implementation of a Joy operator
		*/
		bool context_free(token_t& token, joy_stack& S);

		/**
		* recursively descend into nested list to add a new list
		*/
		void nest_list(joy_stack& S, size_t depth);

		/**
		* recursively descend into nested lists to add token
		* convert list_t to quote_t if a command is added to a list
		*/
		void nest_token(token_t& token, joy_stack& S, joy_t& type, size_t depth);

		/**
		* error state - unwind the state_stack by list_depth
		* notify of any proto stack item deleted
		* print the remaining stack
		*/
		void unwind(joy_stack& S);

		/**
		* state changing operators
		*/
		dictionary_t state_change_atoms {
		{"(*", [&](joy_stack& S) {
			if (state_stack.top() != state_t::comment) {
				state_stack.push(state_t::comment);
			}
		}},
		{"*)", [&](joy_stack& S) {
			if (state_stack.top() != state_t::comment) {
				error(XNOOPENSIGIL, "*)");
			}
			else {
				assert(state_stack.top() == state_t::comment);
				state_stack.pop();
			}
		}},
		{"[", [&](joy_stack& S) { 
			state_stack.push(state_t::list);
			nest_list(S, list_depth);
			++list_depth;
		}},
		{"]", [&](joy_stack& S) { 
			if (list_depth == 0) {
				error(XAGGSIGIL, "]");
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
		*
		* For many operators an implementation can choose whether to make it a Joy03 primitive or define it in a library.
		* Apart from execution speed, to the user it makes no difference as to which choice has been made.
		*/
		dictionary_t context_free_atoms {
		//combinators of aggregate types
		//the combinators in this section expect aggregates below their quotation parameters.
		//the stack is just a list, so any list could serve as the stack, including a list which happens to be on top of the stack.
		{"infra", [&](joy_stack& S) { if (S.has("infra", {joy_t::quote_t, joy_t::group_t})) { infra(S, *this); } }},
		//list operators
		{"uncons", [&](joy_stack& S) { if (S.has("uncons", {joy_t::aggregate_t}))  { uncons(S); } }},
		//general operators
		//ternary
		{"choice", [&](joy_stack& S) { if (S.has("choice", {joy_t::any_t, joy_t::any_t, joy_t::bool_t})) { choice(S); } }},
		// combinators
		{"map", [&](joy_stack& S) { if (S.has("map", {joy_t::quote_t, joy_t::aggregate_t})) { map(S, *this); } }},
		{"reverse", [&](joy_stack& S) { if (S.has("reverse", {joy_t::aggregate_t})) { reverse(S); } }},
		{"step", [&](joy_stack& S) { if (S.has("step", {joy_t::quote_t, joy_t::aggregate_t})) { step(S, *this); } }},
		{"dip", [&](joy_stack& S) {if (S.has("dip", {joy_t::quote_t, joy_t::any_t})) { dip(S, *this); }}},
		{"i", [&](joy_stack& S) { if (S.has("i", {joy_t::group_t})) { i(S, *this); } }}
		};

	};

}
