#pragma once

#include <cassert>
#include <string>

#include <iostream>

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

    inline token_t make_token(std::any arg, joy_t type) {
        return std::make_pair(arg, type);
    }

    inline bool is_joy_bool(const std::string& match) {
        return ((match == "true") || (match == "false"));
    }

    inline bool is_joy_char(const std::string& match) {
        return ((match.size() == 2) && (match[0] == '\''));
    }

    /**
    * test if the pattern is a double
    * allowed: +3.0, 3.2e23, -4.70e+9, -.2E-4, -7.6603
    * not allowed: -1 (integer), +0003 (leading zeros), 37.e88 (dot before the e)
    */
    inline bool is_joy_double(const std::string& match) {
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

    /**
    * test if the pattern is an integer
    * allowed: +1, -1, 0, 10
    * not allowed: +0003   (leading zeros)
    */
    inline bool is_joy_int(const std::string& match) {
        if (is_joy_double(match)) {
            return match.find_first_not_of("+-0123456789") == std::string::npos;
        }
        else {
            return false;
        }
    }

}
