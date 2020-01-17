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
#include "primitives_performance.h"

// [ 1 2 3 4 ] [ + * ] infra
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

		void parse(joy_stack& P, joy_stack& S);

		void parse(joy_stack&& P, joy_stack& S);

		bool parse(token_t& token, joy_stack& S);

		bool is_context_free(token_t& token);

	private:

		/**
		* push down automata context free stack
		*/
		state_stack_t state_stack;

		size_t list_depth{ 0 };
		size_t set_depth{ 0 };

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
		void nest_list(joy_stack& S, size_t depth);

		/**
		* recursively descend into nested lists to add token
		* convert list_t to quote_t if a command is added to a list
		*/
		void nest_token(token_t& token, joy_stack& S, joy_t& type, size_t depth);

		/**
		* error state - unwind the stack by depth
		*/
		void unwind(joy_stack& S);

		//Joy op C++ implementations

		/**
		* The stack can be pushed as a quotation onto the stack by stack, a quotation can be turned into the stack by unstack. 
		* A list on the stack, such as [1 2 3 4] can be treated temporarily as the stack by a quotation, say [+ *] and the combinator infra, with the result [9 4].
		*
		* The infra combinator expects a quotation [P] which will be executed and below that another quotation which normally will be just a list [M].
		* The infra combinator temporarily discards the remainder of the stack and takes the quotation or list [M] to be the stack.
		* It then executes the top quotation [P] which yields a result stack.
		* This resulting stack is then pushed as a list [N] onto the original stack replacing the original quotation or list.
		* Hence any quotation can serve as a complex unary operation on other quotations or lists.
		* 
		*   L  [M]  [P]  infra  =>  L  [N]   :-  [M]  unstack  P  =>  N
		*
		* infra : L1 [P] -> L2
		* Using list L1 as stack, executes P and returns a new list L2.
		* The first element of L1 is used as the top of stack, and after execution of P the top of stack becomes the first element of L2.
		*/
		void infra(joy_stack& S);

		/**
		* map: A [P] -> B
		* Executes P on each member of aggregate A, collects results in sametype aggregate B.
		*/
		token_t map(joy_stack& S);

		/**
		* The dip combinator expects a quotation parameter (which it will consume), and below that one more element. 
		* It saves that element away, executes the quotation on whatever of the stack is left, and then restores the saved element. 
		* So 2 3 4 [+] dip is the same as 5 4. 
		* This single combinator was inspired by several special purpose optimising combinators S', B' and C' in the combinatory calculus, see Peyton Jones (1987, sections 16.2.5 and 16.2.6).
		* dip: X [P] -> ... X
		* Saves X, executes P, pushes X back onto stack.
		*/
		void dip(joy_stack& S);

		/**
	   * step: A [P] -> ...
	   * Sequentially putting members of aggregate A onto a stack M
	   * pasring M that executes P for each member of A stack the result
	   */
		void step(joy_stack& stack);

		/**
		* i: [P] -> ...
		* Executes P. So, [P] i == P
		*/
		void i(joy_stack& S);
	
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
		*
		* For many operators an implementation can choose whether to make it a Joy03 primitive or define it in a library.
		* Apart from execution speed, to the user it makes no difference as to which choice has been made.
		*
		* However, for the minimal set there is no choice but to implement them in the primitives_minimal.h library.
		* namely: unoncs choice infra and the basic arithmetic operators + - * /
		* 
		* Others benefit from performance improvements of being defined in the primitives_performance.h library.
		* namely: reverse
		*
		* Finally, in the current implementation of joycpp, there a number of operators that execute a quoted program internally
		* and, as such, benefit by being defined intimately with the parser class.
		* namely: infra map dip i step
		*/
		dictionary_t context_free_atoms {
		//combinators of aggregate types
		//the combinators in this section expect aggregates below their quotation parameters.
		//the stack is just a list, so any list could serve as the stack, including a list which happens to be on top of the stack.
		{"infra", [&]() { if (require("infra", {joy_t::quote_t, joy_t::group_t})) { infra(root_stack); } }},
		//list operators
		{"uncons", [&]() { if (require("uncons", {joy_t::aggregate_t}))  { uncons(root_stack); } }},
		//general operators
		//ternary
		{"choice", [&]() { if (require("choice", {joy_t::any_t, joy_t::any_t, joy_t::bool_t})) { choice(root_stack); } }},
		// combinators
		{"map", [&]() { if (require("map", {joy_t::quote_t, joy_t::aggregate_t})) { root_stack.push(map(root_stack)); } }},
		{"reverse", [&]() { if (require("reverse", {joy_t::aggregate_t})) { reverse(root_stack); } }},
		{"step", [&]() { if (require("step", {joy_t::quote_t, joy_t::aggregate_t})) { step(root_stack); } }},
		{"dip", [&]() {if (require("dip", {joy_t::quote_t, joy_t::any_t})) { dip(root_stack); }}},
		{"i", [&]() { if (require("i", {joy_t::group_t})) { i(root_stack); } }}
		};

	};

}
