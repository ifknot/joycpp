#include "parse_joy.h"

namespace joy {

	joy_stack parse_joy::tokenize(joy_stack&& tokens) {
		return tokenize_joy_commands(parse_context_free::tokenize(std::move(tokens)));
	}

	bool parse_joy::call(token_t& token, joy_stack& S) {
		auto it = joy_lambda_map.find(std::any_cast<std::string>(token.first));
		if (it != joy_lambda_map.end()) {
			(it->second)(S);
			return true;
		}
		auto jt = public_joy_joy_map.find(std::any_cast<std::string>(token.first));
		if (jt != public_joy_joy_map.end()) {
			joy_stack tokens;
			tokens = tokenize(std::move(jt->second));
			return parse_context_free::parse(tokens, S);
		}
		jt = private_joy_joy_map.find(std::any_cast<std::string>(token.first));
		if (jt != private_joy_joy_map.end()) {
			joy_stack tokens;
			tokens = tokenize(std::move(jt->second));
			return parse_context_free::parse(tokens, S);
		}
		return parse_context_free::call(token, S);
	}

	bool parse_joy::parse(token_t& token, joy_stack& S) {
		switch (joy_state) {
		case joy_state_t::parse: 
			switch (token.second) {
			case joy_t::undef_t:
				joy_state = joy_state_t::candidate;
				command = std::any_cast<std::string>(token.first);
				io.colour(BOLDYELLOW);
				io << "define " + command;
				return true;
			case joy_t::cmd_t:
				return call(token, S);
			default:
				return parse_context_free::parse(token, S);
			}
		case joy_state_t::candidate:
			if (token == "==") {
				joy_state = joy_state_t::define;
				return true;
			}
			else {
				joy_state = joy_state_t::parse;
				return false;
			}
		case joy_state_t::define: 
			if (token == "." || token == ";"){
				io << definition << "end";
				if (validate_tokens(tokenizer::tokenize(std::move(definition)))) {
					public_joy_joy_map[command] = definition;
					definition.clear();
				}
				else {
					error(XDEFNREJECTED, command);
					definition.clear();
				}
				joy_state = joy_state_t::parse;
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
				auto it = joy_lambda_map.find(match);
				if (it != joy_lambda_map.end()) {
					type = joy_t::cmd_t;
					continue;
				}
				auto jt = public_joy_joy_map.find(match);
				if (jt != public_joy_joy_map.end()) {
					type = joy_t::cmd_t;
					continue;
				}
				jt = private_joy_joy_map.find(match);
				if (jt != private_joy_joy_map.end()) {
					type = joy_t::cmd_t;
					continue;
				}
			}
		}
		return std::move(tokens);
	}

	bool parse_joy::validate_tokens(joy_stack&& tokens) {
		bool valid = true;
		for (const auto& t : tokens) {
			if (t.second == joy_t::undef_t) {
				valid = false;
				auto culprit = std::any_cast<std::string>(t.first);
				error(XNOCONVERSION, "command lookup >" + culprit + "< is not recognized");
			}
		}
		return valid;
	}

	void parse_joy::include(std::string&& path) {
		path = path.substr(1, path.size() - 2);
		io << path;
		std::ifstream f(path);
		for (std::string line; std::getline(f, line); ) {
			auto tokens = tokenize(std::move(line));
			root_parse(tokens);
		}
	}

}