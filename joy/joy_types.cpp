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
		{joy_t::lamda_t, "command"},
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

}
