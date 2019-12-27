/**
* Joy is a typed language, there are 2 main groups:
* - simple types: boolean, char, integer, double
* - aggregate types: list, quote, set, string
* joycpp uses some internal types
* - lexeme_t when it is known that a lexeme has parsable pattern 
* - undef_t when it is unknown if a lexeme has a parsable pattern
* giving joycpp 10 fundamental types
* joycpp will need 
* - type checking: is_joy_...(joy_t)
* - sigil managers: add/strip/empty
* - helpers: make_token_...(pattern_t) 
*/
#pragma onceboo

#include <string>
#include <vector>
#include <map>
#include <regex>
#include <cassert>
#include <functional>

#include <iostream>

#include "joy_sigils.h"
#include "joy_tokens.h"

namespace joy {

    enum class joy_t { bool_t, int_t, char_t, double_t, list_t, quote_t, set_t, string_t, lexeme_t, undef_t };

    typedef std::string pattern_t;
    typedef std::pair<pattern_t, joy_t> token_t;
    typedef std::vector<token_t> base_stack_t;
    typedef std::function<void()> function_t;
    typedef std::map<pattern_t, function_t> cpp_dictionary_t;
    
    //string manglers

    /**
    * strip any trailing whitespace
    * TODO: tab, mulitple space, CR, LF, etc.
    */
    inline pattern_t strip_spc(pattern_t& p) {
        return(p.back() == ' ') ? p.substr(0, p.size() - 1) : p;
    }

    /**
    * convert a joy type to its string name (bool, int, char, double, list, quote, set, string, lexeme)
    */
    std::string to_string(joy_t match);

    //type tests

    inline bool is_joy_bool(pattern_t& match) {
        return ((match == TOK_TRUE) || (match == TOK_FALSE));
    }

    inline bool is_joy_char(pattern_t& match) {
        return ((match.size() == 2) && (match[0] == CHAR_OPEN));
    }

    /**
    * test if the pattern is a number (int or double)
    * allowed: +3.0, 3.2e23, -4.70e+9, -.2E-4, -7.6603
    * not allowed: +0003.14   (leading zeros), 37.e88  (dot before the e)
    */
    inline bool is_joy_number(pattern_t& match) {
        return std::regex_match(match, std::regex("[+-]?(?=.)(?:0|[1-9]\\d*)?(?:\.\\d*)?(?:\\d[eE][+-]?\\d+)?"));
    }
    
    /**
    * test if the pattern is an integer
    * allowed: +1, -1, 0, 10
    * not allowed: +0003   (leading zeros)
    */
    inline bool is_joy_int(pattern_t& match) {
        if (is_joy_number(match)) {
            return match.find_first_not_of("+-0123456789") == std::string::npos;
        }
        else {
            return false;
        }
    }

    /**b
    * test if the pattern is a number (int or double)
    * allowed: +3.0, 3.2e23, -4.70e+9, -.2E-4, -7.6603
    * not allowed: -1 (integer), +0003 (leading zeros), 37.e88 (dot before the e)
    */
    inline bool is_joy_double(pattern_t& match) {
        return is_joy_number(match) && !is_joy_int(match);
    }

    //aggreagate managers

    bool is_empty_aggregate(pattern_t& match, char open_sigil, char close_sigil);

    bool is_empty_joy_list(pattern_t match);

    bool is_empty_joy_set(pattern_t match);

    /**
    * test if lexeme is wrapped in the sigil chars
    */
    inline bool is_sigils(pattern_t& match, char open_sigil, char close_sigil) {
          return ((match[0] == open_sigil) && (match.back() == close_sigil));
    }

    inline pattern_t strip_sigils(pattern_t& match, char open_sigil, char close_sigil) {
        assert(is_sigils(match, open_sigil, close_sigil));
        if (is_sigils(match, open_sigil, close_sigil)) {
            return match.substr(2, match.size() - 3);
        }
    }

    inline pattern_t add_sigils(pattern_t& match, char open_sigil, char close_sigil) {
        std::string open{ open_sigil }, close{ close_sigil };
        auto s = strip_spc(match);
        return open + TOK_SPACE + s + TOK_SPACE + close;
    }

    inline bool sigils_contain(pattern_t& test, pattern_t& match, char open_sigil, char close_sigil) {
        if (is_sigils(match, open_sigil, close_sigil)) {
            return test == strip_sigils(match, open_sigil, close_sigil);
        }
        else {
            //TODO: err info e.g. last_error = ERRNUM
            return false;
        }
    }

    inline bool is_joy_list(pattern_t& match) {
        return is_sigils(match, LIST_OPEN, LIST_CLOSE);
    }

    inline bool is_joy_quote(pattern_t& match) {
        return ((is_joy_list(match)) && (!is_empty_aggregate(match, QUOTE_OPEN, QUOTE_CLOSE)));
        return false;
    }

    inline bool is_joy_set(pattern_t& match) {
        return is_sigils(match, SET_OPEN, SET_CLOSE);
    }

    inline bool is_joy_string(pattern_t match) {
        return is_sigils(match, STRING_OPEN, STRING_CLOSE);
    }

    // not sure if is_joy_lexeme is needed yet/ever?


    //make token helpers
   
    inline token_t make_token(pattern_t&& s, joy_t t) {
        return token_t(s, t);
    }

    inline token_t make_string_token(pattern_t&& s) {
        return make_token(add_sigils(s, STRING_OPEN, STRING_CLOSE), joy_t::string_t);
    }

    inline token_t make_quoted_token(pattern_t&& s) {
        return make_token(add_sigils(s, QUOTE_OPEN, QUOTE_CLOSE), joy_t::quote_t);
    }

    /**
    * retrieve the joy type of the joy lexeme 
    */
    joy_t joy_type(pattern_t& match);

}
