#include "joy_types.h"

namespace joy {

    std::map<joy_t, std::string> type_to_string = {
        {joy_t::bool_t, "bool"},
        {joy_t::int_t, "int"},
        {joy_t::char_t, "char"},
        {joy_t::double_t, "double"},
        {joy_t::list_t, "list"},
        {joy_t::quote_t, "quote"},
        {joy_t::set_t, "set"},
        {joy_t::string_t, "string"},
        {joy_t::lexeme_t, "lexeme"},
        {joy_t::undef_t, "undefined"}
    };

    std::map<state_t, std::string> state_to_string = {
        {state_t::parse, " parsing"},
        {state_t::quote, " quote build"},
        {state_t::list, " list build"},
        {state_t::set, " set build"},
        {state_t::string, " string build"}
    };

    std::map<state_t, std::string> state_to_colour = {
        {state_t::parse, BOLDWHITE},
        {state_t::quote, BOLDCYAN},
        {state_t::list, BOLDMAGENTA},
        {state_t::set, BOLDGREEN},
        {state_t::string, BOLDYELLOW}
    };

    std::string to_string(joy_t match) {
        if (type_to_string.count(match)) {
            return type_to_string[match];
        }
        else {
            return "no type to string match";
        }
    }

	std::string to_string(state_t match) {
        if (state_to_string.count(match)) {
            return state_to_string[match];
        }
        else {
            return "no state to string match";
        }
	}

    std::string to_colour(state_t match) {
        if (state_to_colour.count(match)) {
            return state_to_colour[match];
        }
        else {
            return "no state to colour match";
        }
    }

    bool is_empty_aggregate(const pattern_t& match, char open_sigil, char close_sigil) {
        return((match[0] == open_sigil) && (match[2] == close_sigil) && (match[1] == ' '));
    }

    bool is_empty_joy_list(const pattern_t match) {
        return is_empty_aggregate(match, LIST_OPEN, LIST_CLOSE);
    }

    bool is_empty_joy_set(const pattern_t match) {
        return is_empty_aggregate(match, SET_OPEN, SET_CLOSE);
    }

    bool is_empty_joy_string(const pattern_t match) {
        return is_empty_aggregate(match, STRING_OPEN, STRING_CLOSE);
    }

    joy_t joy_type(const pattern_t& match) {
        if (is_joy_bool(match)) return joy_t::bool_t;
        if (is_joy_char(match)) return joy_t::char_t;
        if (is_joy_int(match)) return joy_t::int_t;
        if (is_joy_double(match)) return joy_t::double_t;
        if (is_joy_list(match)) return joy_t::list_t;
        if (is_joy_quote(match)) return joy_t::quote_t;
        if (is_joy_set(match)) return joy_t::set_t;
        if (is_joy_string(match)) return joy_t::string_t;
        //if (is_joy_lexeme(match)) return joy_t::lexem_t;
        return joy_t::undef_t;
    }

}