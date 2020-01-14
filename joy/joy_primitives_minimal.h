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

#include "joy_types.h"

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

    //string joy primitive matching:

    inline bool jlogical(const std::string& match) {
        return ((match == "true") || (match == "false"));
    }

    inline bool jchar(const std::string& match) {
        return ((match.size() == 2) && (match[0] == '\''));
    }

    //char special cases:

    inline bool jchar_space(const std::string& match) {
        return ((match.size() == 1) && (match[0] == '\''));
    }

    inline bool jchar_escape(const std::string& match) {
        return ((match.size() == 3) && (match[0] == '\'') && (match[1] == '\\'));
    }

    inline bool jdouble(const std::string& match) {
        try {
            auto x = std::stod(match);
            return true;
        }
        catch (std::invalid_argument) {
            return false;
        }
        catch (std::out_of_range) {
            return false;
        }
    }

    inline bool jinteger(const std::string& match) {
        if (jdouble(match)) {
            return match.find_first_not_of("+-0123456789") == std::string::npos;
        }
        else {
            return false;
        }
    }

    //token joy type matching

    inline bool jlogical(const token_t& token) {
        return token.second == joy_t::bool_t;
    }

    inline bool jchar(const token_t& token) {
        return token.second == joy_t::char_t;
    }

    inline bool jdouble(const token_t& token) {
        return token.second == joy_t::double_t;
    }

    inline bool jinteger(const token_t& token) {
        return token.second == joy_t::int_t;
    }

    inline bool jnumeric(const token_t& token) {
        return jchar(token) || jdouble(token) || jinteger(token);
    }

    inline bool jset(const token_t& token) {
        return token.second == joy_t::set_t;
    }

    inline bool jstring(const token_t& token) {
        return token.second == joy_t::string_t;
    }

    inline bool jlist(const token_t& token) {
        return token.second == joy_t::list_t;
    }

    inline bool jquote(const token_t& token) {
        // FIX: when able to differentiate quote_t at parsing
        return token.second == joy_t::list_t;
    }

    inline bool jsequence(const token_t& token) {
        return jlist(token) || jquote(token) || jstring(token);
    }

    inline bool jaggregate(const token_t& token) {
        return jsequence(token) || jset(token);
    }

    inline bool jleaf(const token_t& token) {
        return !jaggregate(token);
    }


}