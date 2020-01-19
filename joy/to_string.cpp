#include "to_string.h"

namespace joy {

    /**
    * map joy types to text
    */
    std::map<joy_t, std::string> type_to_string = {
        //grouped types
        {joy_t::undef_t, "undef"},
        {joy_t::any_t, "any"},
        {joy_t::numeric_t, "numeric"},
        {joy_t::group_t, "group"},
        {joy_t::sequence_t, "sequence"},
        {joy_t::aggregate_t, "aggregate"},
        {joy_t::cmd_t, "command"},
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
    };

    /**
    * map parser states to text
    */
    std::map<state_t, std::string> state_to_string = {
        {state_t::comment, " comment"},
        {state_t::parse, " ready"},
        {state_t::quote, " quote"},
        {state_t::list, " list"},
        {state_t::set, " set"}
    };

    /**
    * map states to colour code
    */
    std::map<state_t, std::string> state_to_colour = {
        {state_t::comment, BOLDBLACK},
        {state_t::parse, BOLDWHITE},
        {state_t::quote, BOLDYELLOW},
        {state_t::list, BOLDCYAN},
        {state_t::set, BOLDMAGENTA}
    };

    std::string to_string(const joy_t match) {
        assert(type_to_string.count(match));
        return type_to_string[match];
    }

    std::string to_string(const state_t match) {
        assert(state_to_string.count(match));
        return state_to_string[match];
    }

    std::string to_string(const token_t& token) {
        std::string result;
        switch (token.second) {
        case joy::joy_t::list_t:
        case joy::joy_t::quote_t:
            result += "[ " + to_string(std::any_cast<joy_stack>(token.first)) + "]";
            break;
        case joy::joy_t::set_t:
            break;
        case joy::joy_t::string_t: {
            result += "\"";
            const auto tokens = std::any_cast<joy_stack>(token.first);
            for (auto t : tokens) {
                result.push_back(std::any_cast<char>(t.first));
            }
            result += "\"";
            break;
        }
        case joy::joy_t::bool_t:
            result += (std::any_cast<bool>(token.first)) ? "true" : "false";
            break;
        case joy::joy_t::char_t:
            result += "'" + std::string{ std::any_cast<char>(token.first) };
            break;
        case joy::joy_t::int_t:
            result += std::to_string(std::any_cast<int>(token.first));
            break;
        case joy::joy_t::double_t:
            result += std::to_string(std::any_cast<double>(token.first));
            break;
        case joy::joy_t::undef_t:
        case joy::joy_t::cmd_t:
        default:
            result += std::any_cast<std::string>(token.first);
            break;
        }
        return result + " " + to_string(token.second);
    }

    std::string to_string(const joy_stack& stack) {
        std::string result;
        for (const auto& token : stack) {
            result += to_string(token) + " ";
        }
        return result;
    }

    std::string to_colour(const state_t match) {
        assert(state_to_colour.count(match));
        return state_to_colour[match];
    }

}