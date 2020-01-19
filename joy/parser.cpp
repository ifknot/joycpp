#include "parser.h"

namespace joy {



	parser::parser(joy_stack& stack, io_device& io, std::string path_to_manual) :
		lexer(stack, io, path_to_manual)
	{
		state_stack.push(state_t::parse);
	}

	state_t parser::state() const {
		return state_stack.top();
	}

	joy_stack parser::tokenize(joy_stack&& tokens) {
		return tokenize_context_free_types(lexer::tokenize(std::move(tokens)));
	}

	bool parser::parse(joy_stack& tokens) {
		//for each token
		switch (state_stack.top()) {
		case joy::state_t::comment:
			return true;
		case joy::state_t::parse:
			if !lexer::parse(token) !parse(token)
			break;
		case joy::state_t::quote:
			break;
		case joy::state_t::list:
			break;
		case joy::state_t::set:
			break;
		default:
			throw std::runtime_error("unrecognised state " + to_string(state_stack.top()));
			break;
		}
	}

	void parser::parse(joy_stack& P, joy_stack& S) {
		assert(state_stack.top() == state_t::parse);
		for (auto& token : P) {
			if (!parse(token, S)) {
				break;
			}
		}
	}

	void parser::parse(joy_stack&& P, joy_stack& S) {
		parse(std::move(P), S);
	}

	bool parser::parse(token_t& token, joy_stack& S) {
		if (jundef(token)) {
			return parse_undef(token, S);
		}
		else {
			return parse_defined(token, S);
		}
	}

	bool parser::parse(token_t&& token, joy_stack& S) {
		return parse(std::move(token), S);
	}

	bool parser::is_state_change(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = state_change_atoms.find(std::any_cast<std::string>(token.first));
		if (it != state_change_atoms.end()) {
			return true;
		}
		return false;
	}

	bool parser::is_context_free(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = context_free_atoms.find(std::any_cast<std::string>(token.first));
		if (it != context_free_atoms.end()) {
			return true;
		}
		return false;
	}

	bool parser::parse_undef(token_t& token, joy_stack& S) {
		if (state_change(token, S)) {
			return true;
		}
		if (commenting()) {
			return true;
		}
		if (is_context_free(token) || is_regular(token)) {
			token.second = joy_t::cmd_t;
			return parse_defined(token, S);
		}
		error(XNOCONVERSION, "command lookup >" + to_string(token) + "< " + to_string(token.second) + " is not recognised");
		unwind(S);
		return false;
	}

	bool parser::parse_defined(token_t& token, joy_stack& S) {
		//io << to_string(state_stack.top());
		switch (state_stack.top()) {
		case joy::state_t::parse:
			if (jcmd(token)) {
				return state_change(token, S) || context_free(token, S) || regular(token, S);
			} 
			else {
				S.push(token);
				return true;
			}
		case joy::state_t::list:
		case joy::state_t::quote:
			nest_token(token, S, root_type, list_depth);
			return true;
		case joy::state_t::set:
			// TODO:
			return false;
		case joy::state_t::comment:
			return true;
		default:
			throw std::runtime_error("unrecognised state " + to_string(state_stack.top()));
			break;
		}
	}

	joy_stack parser::tokenize_context_free_types(joy_stack&& tokens) {
		for (auto& [pattern, type] : tokens) {
			if (type == joy_t::undef_t) {
				auto match = std::any_cast<std::string>(pattern);
				auto it = context_free_atoms.find(match);
				if (it != context_free_atoms.end()) {
					type = joy_t::cmd_t;
				}
			}
		}
		return std::move(tokens);
	}

	bool parser::run(token_t& token, joy_stack& S) {
		auto it = context_free_atoms.find(std::any_cast<std::string>(token.first));
		if (it != context_free_atoms.end()) {
			(it->second)(S);
			return true;
		}
		return false;
	}

	bool parser::state_change(token_t& token, joy_stack& S) {
		auto it = state_change_atoms.find(std::any_cast<std::string>(token.first));
		if (it != state_change_atoms.end()) {
			(it->second)(S);
			return true;
		}
		return false;
	}

	bool parser::context_free(token_t& token, joy_stack& S) {
		auto it = context_free_atoms.find(std::any_cast<std::string>(token.first));
		if (it != context_free_atoms.end()) {
			(it->second)(S);
			return true;
		}
		return false;
	}

	void parser::nest_list(joy_stack& S, size_t depth) {
		if (depth == 0) {
			S.emplace_back(joy_stack{}, joy_t::list_t); //default to a list type...
		}
		else {
			assert(jgroup(S.top()));
			nest_list(std::any_cast<joy_stack&>(S.top().first), depth - 1);
		}
	}

	void parser::nest_token(token_t& token, joy_stack& S, joy_t& type, size_t depth) {
		if (depth == 0) {
			S.push(token);
			if (token.second == joy_t::cmd_t) {
				type = joy_t::quote_t; //...unless a joy command is added
				state_stack.pop();
				state_stack.push(state_t::quote);
			}
		}
		else {
			assert(jgroup(S.top())); 
			nest_token(token, std::any_cast<joy_stack&>(S.top().first), S.top().second, depth - 1);
		}
	}

	void parser::unwind(joy_stack& S) {
		print_stack(S, io);
		while (list_depth) {
			state_stack.pop();
			--list_depth;
		}
	}

}
