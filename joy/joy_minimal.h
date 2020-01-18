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
*				B T F │choice   →					│ T or F
*			   [a] [f]│infra    →                  [│a f]
* 
*	2. joy type enquiry take a token X as an argument and return a boolean (extended minimal):
*		
*		jlogical	tests whether X is a logical
*       jchar	    tests whether X is a character
*       jdouble		tests whether X is a double
*		jinteger	tests whether X is an integer
*		jset		tests whether X is a set
*		jstring		tests whether X is a string
*		jlist		tests whether X is a list
*		jleaf		tests whether X is -not- a list
* (extended)
*       jnumeric    tests whether X is int/char/double
*       jsequence   tests whether X is quote/list/string
*       jaggregate  tests whether X is quote/list/string/set
*
* NB argument conformability checking is not done at the primitive level but, in preference, a parser class role
*/
#pragma once

#include <string>
#include <stdexcept>
#include <regex>

#include "joy_types.h"
#include "joy_stack.h"

namespace joy {

    /**
    * uncons unary operators expects a non-empty aggregate. 
    *
    *   [f R]  uncons    =>   f  [R]
    *
    * uncons : A -> F R
    * F and R are the first and the rest of non-empty aggregate A.
    */
    void uncons(joy_stack& S);

	/**
    * General ternary operator: choice
    * The choice operator expects three values on top of the stack, say B, T and F, with F on top. 
    * The third value from the top, B, has to be a truth value. 
    * If it is true, then the choice operator just leaves T on top of the stack, and B and F disappear. 
    * On the other hand, if B is false, then the choice operator just leaves F on top of the stack, and B and T disappear. 
    * (This operator is related to two combinators ifte and branch.)
	* 
    * choice : B T F -> X
	* If B is true, then X = T else X = F.
	*/
	void choice(joy_stack& S);

    

    


}