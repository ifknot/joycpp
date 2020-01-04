#include "joy_utils.h"

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
    };
    
    std::map<state_t, std::string> state_to_string = {
        {state_t::parse, " parse"},
        {state_t::quote, " quote"},
        {state_t::list, " list"},
        {state_t::set, " set"},
        {state_t::string, " string"}
    };

	std::string to_string(joy_t match) {
        assert(type_to_string.count(match));
        return type_to_string[match];
	}

	std::string to_string(state_t match) {
        assert(state_to_string.count(match));
        return state_to_string[match];
	}

    std::string to_string(const token_list_t& list) {
        std::string result;
        for (const auto& token : list) {
            result += to_string(token) + " ";
        }
        return result;
    }

	std::string to_string(const token_t& token) {
        std::string result{"< "};
        switch (token.second) {
        case joy::joy_t::quote_t:
            result += "[ " + to_string(std::any_cast<token_list_t>(token.first)) + "]";
            break;
        case joy::joy_t::list_t:
            result += "[ " + to_string(std::any_cast<token_list_t>(token.first)) + "]";
            break;
        case joy::joy_t::set_t:
            result += "{ " + to_string(std::any_cast<token_list_t>(token.first)) + "}";
            break;
        case joy::joy_t::string_t:
            result += "\"" + std::any_cast<std::string>(token.first) + "\"";
            break;
        case joy::joy_t::bool_t:
            result += (std::any_cast<bool>(token.first)) ?"true" :"false";
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
        default:
            break;
        }
        return result;
	}

}
