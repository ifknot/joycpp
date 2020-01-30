#include "joy_types.h"

namespace joy {

	/**
	* map joy types to text
	*/
	std::map<joy_t, std::string> type_to_string = {
		//grouped types
		{joy_t::undef_t, "undefined"},
		{joy_t::any_t, "any"},
		{joy_t::numeric_t, "numeric"},
		{joy_t::group_t, "group"},
		{joy_t::sequence_t, "sequence"},
		{joy_t::aggregate_t, "aggregate"},
		{joy_t::lambda_t, "c++ lambda"},
		{joy_t::cmd_t, "joy command"},
		//simple types
		{joy_t::bool_t, "boolean"},
		{joy_t::int_t, "int"},
		{joy_t::char_t, "char"},
		{joy_t::double_t, "double"},
		//aggregate types
		{joy_t::list_t, "list"},
		{joy_t::quote_t, "quote"},
		{joy_t::set_t, "set"},
		{joy_t::string_t, "string"},
	};

	std::string to_string(const joy_t match) {
		assert(type_to_string.count(match));
		return type_to_string[match];
	}

	void convert_simple(token_t& token) {
		auto& [pattern, type] = token;
		auto match = std::any_cast<std::string>(pattern);	
		if (jlogical(match)) {
			pattern = (match == "true") ? true : false;
			type = joy_t::bool_t;
			return;
		}
		if (jchar_space(match)) {
			pattern = ' ';
			type = joy_t::char_t;
			return;
		}
		if (jchar_escape(match)) {
			switch (match[2]) {
			case 'n':
				pattern = '\n';
				break;
			case 't':
				pattern = '\t';
				break;
			case 'b':
				pattern = '\b';
				break;
			case 'r':
				pattern = '\r';
				break;
			case 'f':
				pattern = '\f';
				break;
			default:
				break;
			}
			type = joy_t::char_t;
			return;
		}
		if (jchar_decimal(match)) {
			pattern = static_cast<char>(stoi(match.substr(2, 3)));
			type = joy_t::char_t;
			return;
		}
		if (jchar(match)) {
			pattern = match[1];
			type = joy_t::char_t;
			return;
		}
		if (jinteger(match)) {
			pattern = stoi(match);
			type = joy_t::int_t;
			return;
		}
		if (jdouble(match)) {
			pattern = stod(match);
			type = joy_t::double_t;
			return;
		}
	}

}
