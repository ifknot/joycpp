#include "parse_joy.h"

namespace joy {
	parse_joy::parse_joy(joy_stack& stack, io_device& io, std::string path_to_manual) :
		defn_state(defn_state_t::parse),
		parse_context_free(stack, io, path_to_manual)
	{}

	joy_stack parse_joy::tokenize(joy_stack&& tokens) {
		return tokenize_joy_commands(parse_context_free::tokenize(std::move(tokens)));
	}

	bool parse_joy::call(token_t& token, joy_stack& S) {
		auto it = joy_joy_map.find(std::any_cast<std::string>(token.first));
		if (it != joy_joy_map.end()) {
			joy_stack tokens;
			tokens = tokenize(std::move(it->second));
			return parse_context_free::parse(tokens, S);
		}
		return parse_context_free::call(token, S);
	}

	bool parse_joy::parse(token_t& token, joy_stack& S) {
		switch (defn_state) {
		case defn_state_t::parse: 
			switch (token.second) {
			case joy_t::undef_t:
				defn_state = defn_state_t::candidate;
				command = std::any_cast<std::string>(token.first);
				io.colour(BOLDYELLOW);
				io << "define " + command;
				return true;
			case joy_t::cmd_t:
				return this->call(token, S);
			//case joy_t::end_t:
				//error(XAGGSIGIL, std::any_cast<std::string>(token.first));
				//return false;
			default:
				return parse_context_free::parse(token, S);
			}
		case defn_state_t::candidate:
			if (token == BEGINDEF) {
				defn_state = defn_state_t::define;
				return true;
			}
			else {
				defn_state = defn_state_t::parse;
				return false;
			}
		case defn_state_t::define: 
			if (token.second == joy_t::end_t){
				io << definition << "end";
				joy_joy_map[command] = definition;
				definition.clear();
				defn_state = defn_state_t::parse;
			}
			else {
				definition += " " + joy_stack::to_string(token);
			}
			return true;
		default:
			return false;
		}
		
	}

	joy_stack parse_joy::tokenize_joy_commands(joy_stack&& tokens) {
		for (auto& [pattern, type] : tokens) {
			if (type == joy_t::undef_t) {
				auto match = std::any_cast<std::string>(pattern);
				auto it = joy_joy_map.find(match);
				if (it != joy_joy_map.end()) {
					type = joy_t::cmd_t;
				}
			}
		}
		return std::move(tokens);
	}

}