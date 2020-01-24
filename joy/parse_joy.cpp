#include "parse_joy.h"

namespace joy {

	joy_stack parse_joy::tokenize(joy_stack&& tokens) {
		return parse_context_free::tokenize(std::move(tokens));
	}

	bool parse_joy::parse(token_t& token, joy_stack& S) {
		switch (defn_state) {
		case defn_state_t::joy:
			break;
		case defn_state_t::candidate:
			break;
		case defn_state_t::define:
			break;
		default:
			break;
		}
		return parse_context_free::parse(token, S);
	}

}