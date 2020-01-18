﻿/**
* Joy03 Combinators Library
*
* It is useful to classify Joy atomic programs into categories depending on what kind of function they denote. 
* A coarse classification distinguishes just three: 
* 1. literals 
* 2. the operators
* 3. the combinators.
*
* Combinator atoms require quoted programs atop the stack. 
* Combinators also denote functions which are defined only on states having the appropriate number of quoted programs on top of the stack. 
* They yield as values another state which depends on the argument state, including the quoted programs, and on the combinator itself.
*/
#pragma once

#include "joy_stack.h"

namespace joy {

	/**
	* Joy has certain devices called combinators which cause the execution of quoted programs that are on top of the stack.
	* One of the simplest is the i combinator.
	* Its effect is to execute a single program on top of the stack, and nothing else.
	* Syntactically speaking, its effect is to remove the quoting square brackets and thus to expose the quoted program for execution.
	*
	* [a]│i   →  │a
	* i: [P] -> ...
	* Executes P. So, [P] i == P
	*/
	template<typename parser_t>
	void i(joy_stack& S, parser_t& parse) {
		auto P = std::any_cast<joy_stack&>(S.top().first);
		S.pop();
		parse(P, S);
	}

	/**
	* The dip combinator expects a quotation parameter (which it will consume), and below that one more element.
	* It saves that element away, executes the quotation on whatever of the stack is left, and then restores the saved element.
	* So 2 3 4 [+] dip is the same as 5 4.
	* This single combinator was inspired by several special purpose optimising combinators S', B' and C' in the combinatory calculus, see Peyton Jones (1987, sections 16.2.5 and 16.2.6).
	*
	* a [f]│dip      →       │f a
	* dip: X [P] -> ... X
	* Saves X, executes P, pushes X back onto stack.
	*/
	template<typename parser_t>
	void dip(joy_stack& S, parser_t& parse) {
		S.swap(); //flip [P] and X
		const auto X = S.top(); //save token
		S.pop();
		i(S, parse); //execute [P]
		S.push(X); //restore X
	}

	/**
	* The times unary combinator expects a numeric value below its quotation parameter and executes its quotation parameter as many times as indicated by the numeric value.
	* If the value is zero or less, then the quotation is not executed at all.
	*
	* n [ops]│times    →              ops⍣n│
	* times : N[P] ->...
	* N times executes P.
	*/
	//template<typename parser_t>
	//void times(joy_stack& S, parser_t& parse) {

	/**
	* The step combinator can be used to access all elements of an aggregate in sequence.
	* For strings and lists this means the order of their occurrence, for sets it means the underlying order.
	* The following will step through the members of the second list and swons them into the initially empty first list.
	* []  [2 8 3 6 5]  [swons]  step
	* The effect is to reverse the non-empty list, yielding [5 6 3 8 2].
	*
	* [a b ...] [p]│step     →     │a p b p ...
	* step: A [P] -> ...
	* Sequentially putting members of aggregate A onto a stack M
	* pasring M that executes P for each member of A stack the result
	*/
	template<typename parser_t>
	void step(joy_stack& S, parser_t& parse) {
		auto P = std::any_cast<joy_stack&>(S.top().first); //get the program
		S.pop();
		auto A = std::any_cast<joy_stack&>(S.top().first); //get the aggregate
		S.pop();
		joy_stack M;
		for (auto a : A) {
			M.push(a);
			for (auto p : P) {
				M.push(p);
			}
		}
		parse(M, S);
	}

	/**
	* The map combinator expects an aggregate value on top of the stack, and it yields another aggregate of the same size.
	* The elements of the new aggregate are computed by applying the quoted program to each element of the original aggregate.
	* [a b ...] [f]│map     →       [f¨a b ...]│
	* map: A [P] -> B
	* Executes P on each member of aggregate A, collects results in sametype aggregate B.
	*/
	template<typename parser_t>
	void map(joy_stack& S, parser_t& parse) {
		auto sz = size(S.sat(1)); //aggregate size
		auto type = S.sat(1).second; //get the return aggregate type
		step(S, parse);
		joy_stack M; //fresh stack to work with
		while (sz--) { //copy off the results of step
			M.push(S.top());
			S.pop();
		}
		M.stack(); //convert the stack to a list [N]
		M.top().second = type;
		S.push(M.top());  //push [N] as the result
	}

}