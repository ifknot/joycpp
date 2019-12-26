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

    std::string to_string(joy_t match) {
        if (type_to_string.count(match)) {
            return type_to_string[match];
        }
        else {
            return "no type to string match";
        }
    }

    joy_t joy_type(pattern_t& match) {
        
        return joy_t::undef_t;

    }

}