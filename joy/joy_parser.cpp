#include "joy_parser.h"

namespace joy {

	joy_parser::joy_parser(joy_stack& stack, io_device& io, std::string path_to_manual) :
		parser(stack, io, path_to_manual)
	{
		//lib loading process
	}

	void joy_parser::parse(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		parse_joy(tokens);
	}

	void joy_parser::parse_joy(joy_stack& P) {
		for (auto& token : P) {
			if (!parse_joy(token)) {
				break;
			}
		}
	}

	bool joy_parser::parse_joy(token_t& token) {
		switch (state_stack.top()) {
		case joy::state_t::parse:
			return(user_token(token) || joy_token(token) || parser::parse(token, root_stack));
		default:
			return parser::parse(token, root_stack);
		}
	}

	bool joy_parser::user_token(token_t& token) {
		if (token.second == joy_t::undef_t) {
			auto it = joy_user.find(std::any_cast<std::string>(token.first));
			if (it != joy_user.end()) {
				token.first = it->second;
				return parser::parse(token, root_stack);
			}
		}
		return false;
	}

	bool joy_parser::joy_token(token_t& token) {
		if (token.second == joy_t::undef_t) {
			auto it = joy_core.find(std::any_cast<std::string>(token.first));
			if (it != joy_core.end()) {
				token.first = it->second;
				return parser::parse(token, root_stack);
			}
		}
		return false;
	}

}