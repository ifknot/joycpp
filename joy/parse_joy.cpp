#include "parse_joy.h"

namespace joy {

	parse_joy::parse_joy(joy_stack& stack, io_device& io, std::string path_to_manual) :
		parse_context_free(stack, io, path_to_manual)
	{
		//echo_state = echo_state_t::linenumber;
		include("joy_libs/usrlib.joy");
		io.ok();
		io.colour(BOLDWHITE);
		io << " ready";
	}
	joy_stack parse_joy::tokenize(joy_stack& tokens) {
		return tokenize_joy_commands(parse_context_free::tokenize(tokens));
	}

	std::string parse_joy::help() {
		std::string result;
		for (const auto& [key, lamda] : joy_lambda_map) {
			result += key + " ";
		}
		for (const auto& [key, lexeme] : public_joy_joy_map) {
			result += key + " ";
		}
		return result + parse_context_free::help();
	}

	std::string parse_joy::_help() {
		std::string result;
		for (const auto& [key, lambda] : joy_lambda_map) {
			result += key + " ";
		}
		for (const auto& [key, lexeme] : private_joy_joy_map) {
			result += key + " ";
		}
		return result + parse_context_free::help();
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
		case joy_state_t::abort:
			return false;
		case joy_state_t::parse: 
			switch (token.second) {
			case joy_t::undef_t:
				command = module_name + std::any_cast<std::string>(token.first);
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
					if (private_access) {
						private_joy_joy_map[command] = definition;
					}
					else {
						public_joy_joy_map[command] = definition;
					}
				}
				else {
					error(XDEFNREJECTED, command);
				}
				if (token == "." || token == "END") {
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
			joy_state = joy_state_t::parse;
			return true;
		default:
			return false;
		}
		
	}

	bool parse_joy::parse(joy_stack& P, joy_stack& S) {
		for (auto& token : P) {
			if (!parse(token, S)) {
				if (joy_state == joy_state_t::abort) {
					return false;
				}
				else {
					no_conversion(P);
					return false;
				}
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
				if (private_access & jt != private_joy_joy_map.end()) {
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
		std::ifstream f(path);
		echo(0, path);
		auto prior_state = autoput_state;
		autoput_state = autoput_state_t::none;
		line_number_stack.push(line_number);
		line_number = 1;
		if (f) {
			for (std::string line; std::getline(f, line); ) {
				auto tokens = tokenize(line);
				if (!root_parse(tokens)) {
					error(XABORT, path + " at line " + std::to_string(line_number - 1));
					break;
				}
			}
		}
		else {
			error(XNOFILE, "include " + path);
		}
		autoput_state = prior_state;
		line_number = line_number_stack.top();
		line_number_stack.pop();
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
		case autoput_state_t::none:
		default:
			break;
		}
	}

}