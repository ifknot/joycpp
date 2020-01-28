#include "parse_joy.h"

namespace joy {

	joy_stack parse_joy::tokenize(joy_stack& tokens) {
		return tokenize_joy_commands(parse_context_free::tokenize(tokens));
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
			tokens = tokenize(jt->second);
			return parse_context_free::parse(tokens, S);
		}
		jt = private_joy_joy_map.find(std::any_cast<std::string>(token.first));
		if (jt != private_joy_joy_map.end()) {
			joy_stack tokens;
			tokens = tokenize(jt->second);
			return parse_context_free::parse(tokens, S);
		}
		return parse_context_free::call(token, S);
	}

	bool parse_joy::parse(token_t& token, joy_stack& S) {
		switch (joy_state) {
		case joy_state_t::parse: 
			switch (token.second) {
			case joy_t::undef_t:
				command = module_name + std::any_cast<std::string>(token.first);
				io.colour(BOLDYELLOW);
				io << "DEFINE " + command;
				joy_state = joy_state_t::candidate;
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
				error(XDEFNREJECTED, command + " missing ==");
				joy_state = joy_state_t::parse;
				return false;
			}
		case joy_state_t::define: 
			if (token == "." || token == ";" || token == "END") {
				if (validate_tokens(tokenizer::tokenize(definition))) {
					if (public_def) {
						public_joy_joy_map[command] = definition;
					}
					else {
						private_joy_joy_map[command] = definition;
					}
				}
				else {
					error(XDEFNREJECTED, command);
				}
				io << definition;
				if (token == "." || token == "END") {
					io << "END";
					module_name.clear();
				}
				definition.clear();
				joy_state = joy_state_t::parse;
			}
			else {
				definition += " " + joy_stack::to_string(token);
			}
			return true;
		case joy_state_t::module:
			assert(jundef(token));
			module_name = std::any_cast<std::string>(token.first) + ".";
			io << module_name;
			joy_state = joy_state_t::parse;
			return true;
		default:
			return false;
		}
		
	}

	bool parse_joy::parse(joy_stack& P, joy_stack& S) {
		break_out = false;
		for (auto& token : P) {
			if (!parse(token, S)) {
				no_conversion(P);
				return false;
			}
			if (break_out) { //breaks out of the parsing loop
				break;
			}
		}
		return true;
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
			auto tokens = tokenize(line);
			root_parse(tokens);
		}
	}

	void parse_joy::autoput(joy_stack& S) {
		switch (autoput_state) {
		case autoput_state_t::top:
			if (S.size()) {
				print_top(S, io);
			}
			break;
		case autoput_state_t::stack:
			print_stack(S, io);
			break;
		default:
			break;
		}
	}

}