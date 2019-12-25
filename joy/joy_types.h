/**
* Joy is a typed language, there are 2 main groups:
* - simple types: boolean, char, integer, double
* - aggregate types: list, quote, set, string
* joycpp uses some internal types
* - lexeme_t when it is known that a lexeme has parsable pattern 
* - undef_t when it is unknown if a lexeme has a parsable pattern
* joycpp will need 
* - type checking: is_joy_...(joy_t)
* - helpers: make_token_...(pattern_t) 
*/
#pragma once

#include <string>
#include <vector>
#include <map>

#include "joy_sigils.h"
#include "joy_tokens.h"

namespace joy {

    enum class joy_t { bool_t, int_t, char_t, double_t, list_t, quote_t, set_t, string_t, lexeme_t, undef_t };

    typedef std::string pattern_t; 
    typedef std::pair<pattern_t, joy_t> token_t;
    typedef std::vector<token_t> base_stack_t;

    std::string to_string(joy_t match);

    inline pattern_t strip_spc(pattern_t& p) {
        if(p.back() == ' ') {
            return p.substr(0, p.size() - 1);
        }
        else {
            return p;
        }
    }

    inline bool is_joy_bool(pattern_t& p) {
        if ((p == TOK_TRUE ) || (p == TOK_FALSE)) {
            return true;
        }
        else {
            return false;
        }
    }
   
    inline token_t make_token(pattern_t&& s, joy_t t) {
        return token_t(s, t);
    }

    inline token_t make_quoted_token(pattern_t&& s) {
        s = QUOTE_OPEN + SPC + s;
        s = strip_spc(s);
        if (s[s.size() - 1] == ' ') {
            return make_token(s + QUOTE_CLOSE, joy_t::quote_t);
        }
        else {
            return make_token(s + SPC + QUOTE_CLOSE, joy_t::quote_t);
        }
    }

    joy_t joy_type(pattern_t& p);

}
