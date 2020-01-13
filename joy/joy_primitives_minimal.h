/**
* Joy language primitives - minimal required C++ implementations
* The minimal set of Joy operators from which the remainder of the language can be built:
*	Basic: uncons, swap, dup, pop, choice, infra, stack, unstack
*	IO: putch, display, include, quit
*	Type: logical, char, integer, string, list
* However, joycpp implements an extended joy stack class wherein all of the stack operators are implemented in C++
* The IO operators require an environment and are best as parser class member functions
* The remaining primitives then fall into 2 groups 
*	1. act on a stack (for swap, dup, pop, stack, unstack see joy_stack.h)
*
*  minimal operators ──┐
*    top of stack ───┐ │
*                    ↓ ↓
*				 [a b]│uncons   →              a [b]│
*				  T/F │choice   →					│ T/F
*			   [a] [f]│infra    →                  [│a f]
* 
*	2. joy type enquiry take a token X as an argument and return a boolean (extended minimal):
*		integer		tests whether X is an integer.
*		char		tests whether X is a character.
*		logical		tests whether X is a logical.
*		set			tests whether X is a set.
*		string		tests whether X is a string.
*		list		tests whether X is a list.
*		leaf		tests whether X is -not- a list.
*		float		tests whether X is a float.
*
* NB argument conformability checking is not done at the primitive level but, in preference, a parser class role
*/
#pragma once

namespace joy {

	/**
	* choice : B T F -> X
	* If B is true, then X = T else X = F.
	*/
	//void choice(joy_stack& S);

	/**
	* infra : L1 [P] -> L2
	* Using list L1 as stack, executes P and returns a new list L2.
	* The first element of L1 is used as the top of stack, and after execution of P the top of stack becomes the first element of L2.
	*/
	//void infra(joy_stack& S);

	/**
	* uncons : A -> F R
	* F and R are the first and the rest of non-empty aggregate A.
	*/
	//void uncons(joy_stack& S);

	
	//bool integer

	//bool char

	//bool logical

	//bool set

	//bool string

	//bool list

	//bool leaf

	//bool float

}