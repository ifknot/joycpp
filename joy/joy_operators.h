/**
* Joy03 Operators Library
*
* It is useful to classify Joy atomic programs into categories depending on what kind of function they denote.
* A coarse classification distinguishes just three:
* 1. literals
* 2. the operators
* 3. the combinators.
*
* The operator atoms are those which look like n-ary operators in other languages. 
* They include the operations such as for addition and the other arithmetical operations, and for the various operations on other types. 
* Like all programs, operators denote functions from states to states, but the functions are not defined on all states. 
* An n-ary operator (such as the binary addition operator) denotes a function which is defined only on states whose stack component has n items (such as two integers) on top.
* The function yields as value another state which is like the argument state except that the n items on the stack have been replaced by the result (such as the sum).
*
* Also included as operators are those that involve input and output such as get and put.
*
* Stack operators such as dup, pop, swap, etc. are member functions of the joy_stack class
*
* For many operators an implementation can choose whether to make it a Joy03 primitive or define it in a library.
* Apart from execution speed, to the user it makes no difference as to which choice has been made.
* Those benefiting from performance improvements are defined here.
*
* performance operators ──┐
*       top of stack ───┐ │
*                       ↓ ↓
*			   	  [a b c]│reverse  →            [c b a]│
*                 [a ...]│size     →                 ≢⍵│
*
*/
#pragma once

#include "joy_stack.h"
#include "io_device.h"

namespace joy {

    /**
    * [a b c]│reverse  →   [c b a]│
    */
    void reverse(joy_stack& S);

    /**
    * [a ...]│size    →   ≢⍵│
    */
    void size(joy_stack& S);

    /**
    * syntactice sugar
    */
    inline int size(token_t& token) {
        return static_cast<int>(std::any_cast<joy_stack>(token.first).size());
    }

    /**
    * Joy . operator
    * print top of stack S in left:right format to an io device
    */
    void print_top(const joy_stack& S, io_device& io);

    /**
    * Joy .s operator
    * print stack S size and S contents in top:down format to an io device
    */
    void print_stack(const joy_stack& S, io_device& io);

    /**
    * manual: ->
    * Writes the loaded manual of all Joy primitives to an io device.
    */
    void manual(std::map<std::string, std::string>& joy_manual, io_device& io);

    /**
    * helpdetail: [ S1 S2 .. ]
    * @note joycpp uses ? as a synonym convenience
    * Gives brief help on each symbol S in the list.
    */
    void helpdetail(const joy_stack& S, std::map<std::string, std::string>& joy_manual, io_device& io);
}
