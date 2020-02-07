#pragma once

#include <string>
#include <stdexcept>
#include <regex>

#include "joy_types.h"
#include "joy_stack.h"

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

	/**
	* The stack can be pushed as a quotation onto the stack by .stack()
	* .. b a│stack    →    .. b a [a b ..]│
	* A quotation can be turned into the stack by .unstack()
	* . [a b]│unstack  →                b a│
	* Therefore, a list on the stack, e.g. [1 2 3 4], can be treated temporarily as the stack by a quotation, say [+ *] and the combinator infra, with the result [9 4].
	*
	* The infra combinator expects a quotation [P] which will be executed and below that another quotation which normally will be just a list [M].
	* The infra combinator temporarily discards the remainder of the stack and takes the quotation or list [M] to be the stack.
	* It then executes the top quotation [P] which yields a result stack.
	* This resulting stack is then pushed as a list [N] onto the original stack replacing the original quotation or list.
	* Hence any quotation can serve as a complex unary operation on other quotations or lists.
	*
	*   L  [M]  [P]  infra  =>  L  [N]   :-  [M]  unstack  P  =>  N
	*
	* [a] [f]│infra    →     [│a f]
	* infra : L1 [P] -> L2
	* Using list L1 as stack, executes P and returns a new list L2.
	* The first element of L1 is used as the top of stack, and after execution of P the top of stack becomes the first element of L2.
	*/
	template<typename parser_t>
	void infra(joy_stack& S, parser_t& parse) {
		auto P = S.top(); // get the program
		S.pop();
		joy_stack M; //fresh stack to work with
		M.push(S.top()); //get the list
		S.pop();
		M.unstack(); //make the list the stack
		M.push(P); // push the program [P]
		//i(M, parse); //execute P
		{ //in-line i to liberate dependency 
			auto P = joy_cast<joy_stack&>(M.top());
			M.pop();
			parse(P, M);
		}
		M.stack(); //convert the stack to a list [N]
		S.push(M.top()); //return [N] as the result
	}

}