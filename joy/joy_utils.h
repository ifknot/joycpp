#pragma once

#include <cassert>
#include <string>

#include "joy_types.h"

namespace joy {

    /**
   * convert a joy type to its string name (bool, int, char, double, list, quote, set, string)
   */
    std::string to_string(joy_t match);

    /**
    * convert a parser state type to its string name ( parse, quote, list, set, string)
    */
    std::string to_string(state_t match);

    /**
    * convert a list of tokens to its string representation
    */
    std::string to_string(const token_list_t& list);

    /**
    * convert a token to its string representation
    */
    std::string to_string(const token_t& token);

}
