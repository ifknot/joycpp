/**
* Convert joy types, states, tokens and stacks to string representation.
*/
#pragma once

#include <string>

#include "colour_codes.h"
#include "joy_types.h"
#include "joy_stack.h"

namespace joy {

    /**
    * Convert a joy type to its string name (bool, int, char, double, list, quote, set, string)
    */
    std::string to_string(const joy_t match);

    /**
    * Converts a parser state type to its string name ( parse, quote, list, set, string)
    * May throw std::bad_alloc from the std::string constructor.
    */
    std::string to_string(const state_t match);

    /**
    * Converts a joy token's std::any to std::string as defined by the tokens joy type for all joy types
    * May throw std::bad_alloc from the std::string constructor.
    */
    std::string to_string(const token_t& token);

    /**
    * Converts a joy stack to std::string as defined by its tokens joy type for all joy types
    * May throw std::bad_alloc from the std::string constructor.
    */
    std::string to_string(const joy_stack& stack);

    /**
    * convert a state to its colour code
    */
    std::string to_colour(const state_t match);

}
