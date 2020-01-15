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

    /**
    * expects 2 numeric types atop the stack or risk undefined behaviour
    * result is always cast to match joy_t of first arguement
    * NB there is no warning if any loss of precision
    */
    token_t operator+(const token_t& a, const token_t& b);

    token_t operator-(const token_t& a, const token_t& b);

    token_t operator*(const token_t& a, const token_t& b);

    token_t operator/(const token_t& a, const token_t& b);

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
        //prevent stod from parsing the joy combinator infra as infinity
        if (std::regex_match(match, std::regex("[+-]?(?=.)(?:0|[1-9]\\d*)?(?:\.\\d*)?(?:\\d[eE][+-]?\\d+)?"))) {
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
        return token.second == joy_t::quote_t;
    }

    inline bool jundef(const token_t& token) {
        return token.second == joy_t::undef_t;
    }

    inline bool jcmd(const token_t& token) {
        return token.second == joy_t::cmd_t;
    }

    inline bool jgroup(const token_t& token) {
        return jlist(token) || jquote(token);
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