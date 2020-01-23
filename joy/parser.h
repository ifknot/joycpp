/**
* Chomsky type 2 context free grammar parser
* i.e. can be decided by a push down automaton
* Uses:
* 1. state stack 
* 2. nesting depth counter
* 3. root stack and temporary stacks
* 4. map of string Joy operator to lambda function mapping for context free C++ implemented Joy operators
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
// [ [ 1 2 3 ] [ 4 5 6 ] ] [ [ dup dup * * ] map ] map .

namespace joy {

	class parser : public lexer {

		using state_stack_t = std::stack<state_t>;

	public:

		parser(joy_stack& stack, io_device& io, std::string path_to_manual);

		state_t state() const;

		using lexer::tokenize;

		/**
		* cascade tokens to tokenizer type id the simple type 
		* then lexer type id any regular commands
		* then parser type id any context free commands
		*/
		virtual joy_stack tokenize(joy_stack&& tokens) override;

		/**
		* entry point for tokenized stack
		*/
		bool root_parse(joy_stack& tokens);

		/**
		* unwind the stack then call lexer::no_conversion
		* then cascade to lexer to print errors
		*/
		virtual void no_conversion(joy_stack& tokens) override;

		/**
		* endow anonymous functor behavior for context free parsing
		* so that can use context free libraries e.g joy_combinators
		*/
		inline void operator()(joy_stack& P, joy_stack& S) {
			parse(P, S);
		}

	protected:

		/**
		* TODO: performance comparison if reserve large stack space on startup
		* e.g. quick sort a large file
		* push down automata's context free stack
		*/
		state_stack_t state_stack;

	private:

		/**
		* the number of nested lists/quotes
		*/
		size_t list_depth{ 0 };

		/**
		* nested parse a quoted program P against a stack S
		*/
		bool parse(joy_stack& P, joy_stack& S);

		/**
		* nested parse a token against a stack S
		*/
		bool parse(token_t& token, joy_stack& S);

		/**
		* tokenize context free expression Joy command types
		*/
		joy_stack tokenize_context_free_types(joy_stack&& tokens);

		/**
		* TODO: rename call()
		* executes context free Joy operators implemented as C++ lambda
		* operator matching function and execute if match return true otherwise return false
		*/
		bool exec_context_free(token_t& token, joy_stack& S);

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
		* test if the token is either an open or close sigil 
		*/
		bool is_sigil(token_t& token, std::string&& open_sigil, std::string&& close_sigil);

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
		{"*)", [&](joy_stack& S) { error(XNOOPENSIGIL, "*)"); }},
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
		}},
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
