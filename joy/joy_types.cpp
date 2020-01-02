#include "joy_types.h"

namespace joy {

    std::map<joy_t, std::string> type_to_string = {
        //simple types
        {joy_t::bool_t, "bool"},
        {joy_t::int_t, "int"},
        {joy_t::char_t, "char"},
        {joy_t::double_t, "double"},
        //aggregate types
        {joy_t::list_t, "list"},
        {joy_t::quote_t, "quote"},
        {joy_t::set_t, "set"},
        {joy_t::string_t, "string"},
        //mixed types
        {joy_t::number_t, "number"},
        {joy_t::aggregate_t, "aggreagte"},
        //abstract types
        {joy_t::lexeme_t, "lexeme"},
        {joy_t::undef_t, "undefined"},
        {joy_t::any_t, "any"}
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

    std::string to_string(const token_t& token) {
        return token.first + " " + to_string(token.second);
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

    double as_double(const token_t& token) {
        switch (token.second) {
        case joy::joy_t::bool_t:
            return (token.first == "true") ? 1.0 : 0.0;
        case joy::joy_t::char_t:
            return token.first[1];
        case joy::joy_t::int_t:
        case joy::joy_t::double_t:
            return stod(token.first);
        default:
            throw std::runtime_error("no conversion for " + to_string(token.second) + " to double");

            break;
        }
    }

    token_t joy_cast(joy_t type, const token_t& token) {
        if (type == token.second) {
            return token;
        }
        switch (type) {
        case joy::joy_t::bool_t:
            switch (token.second) {
            case joy_t::int_t: //int to bool 
                return (stoi(token.first) == 0) ? make_token("false", joy_t::bool_t) : make_token("true", joy_t::bool_t);
            default:
                throw std::runtime_error("no conversion for " + to_string(token.second) + " to " + to_string(type));
            }
            break;
        case joy::joy_t::int_t:
            switch (token.second) {
            case joy_t::bool_t: //bool to int 
                return (token.first == "true") ? make_token("1", joy_t::int_t) : make_token("0", joy_t::int_t);
            case joy_t::double_t: //double to int
                return make_token(std::to_string(stoi(token.first)), joy_t::int_t);
            case joy_t::char_t: //char to int
                return make_token(std::to_string(static_cast<int>(token.first[1])), joy_t::int_t);
            default:
                throw std::runtime_error("no conversion for " + to_string(token.second) + " to " + to_string(type));
            }
            break;
        case joy::joy_t::char_t:
            switch (token.second) {
            case joy_t::double_t: //double to char
            case joy_t::int_t: { //int to char
                auto c = static_cast<char>(stoi(token.first));
                return make_char(std::string{ c }, joy_t::char_t);
            }
            default:
                throw std::runtime_error("no conversion for " + to_string(token.second) + " to " + to_string(type));
            }
            break;
        case joy::joy_t::double_t:
            switch (token.second) {
            case joy_t::int_t: //int to double
                return make_token(std::to_string(stod(token.first)), joy_t::double_t);
            default:
                throw std::runtime_error("no conversion for " + to_string(token.second) + " to " + to_string(type));
            }
            break;
        case joy::joy_t::string_t:
            switch (token.second) {
            case joy_t::bool_t: //bool to string 
            case joy_t::int_t: //int to string 
            case joy_t::double_t: //double to string
                return make_token(add_sigils(token.first, STRING_OPEN, STRING_CLOSE), joy_t::string_t);
            case joy_t::char_t: //char to string 
                return make_token(add_sigils(std::string{ token.first[1] }, STRING_OPEN, STRING_CLOSE), joy_t::string_t);
            default:
                throw std::runtime_error("no conversion for " + to_string(token.second) + " to " + to_string(type));
            }
            break;
        case joy::joy_t::quote_t:
        case joy::joy_t::set_t:
        case joy::joy_t::number_t:
        case joy::joy_t::aggregate_t:
        case joy::joy_t::lexeme_t:
        case joy::joy_t::undef_t:
        case joy::joy_t::any_t:
        default:
            switch (token.second) {
            default:
                throw std::runtime_error("no conversion for " + to_string(token.second) + " to " + to_string(type));
            }
            break;
        }
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