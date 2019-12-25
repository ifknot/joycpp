/**
* Joy is a typed language, there are 2 main groups:
* - simple types: boolean, char, integer, double
* - aggregate types: list, quote, set, string
* joycpp uses some internal types
* - lexeme_t when it is known that a lexeme has parsable pattern 
* - undef_t when it is unknown if a lexeme has a parsable pattern
*/
#pragma once

#include <string>
#include <vector>

#include "joy_sigils.h"

namespace joy {

    enum class joy_t { bool_t, int_t, char_t, double_t, list_t, quote_t, set_t, string_t, lexeme_t, undef_t };

    typedef std::string pattern_t; 
    typedef std::pair<pattern_t, joy_t> token_t;
    typedef std::vector<token_t> base_stack_t;

	inline std::string to_string(joy_t match) {
		switch (match) {
        case joy::joy_t::bool_t:
            return "bool";
        case joy::joy_t::int_t:
            return "int";
        case joy::joy_t::char_t:
            return "char";
        case joy::joy_t::double_t:
            return "double";
        case joy::joy_t::list_t:
            return "list";
        case joy::joy_t::quote_t:
            return "quote";
        case joy::joy_t::set_t:
            return "set";
        case joy::joy_t::string_t:
            return "string";
        case joy::joy_t::lexeme_t:
            return "pattern";
        case joy::joy_t::undef_t:
        default:
            return "undefined";
		}
	}

    inline token_t make_token(pattern_t s, joy_t t) {
        return token_t(s, t);
    }

    inline token_t make_quoted_token(pattern_t s) {
        s = QUOTE_OPEN + SPC + s;
        if (s[s.size() - 1] == ' ') {
            return make_token(s + QUOTE_CLOSE, joy_t::quote_t);
        }
        else {
            return make_token(s + SPC + QUOTE_CLOSE, joy_t::quote_t);
        }
    }

}
