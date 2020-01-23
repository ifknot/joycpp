#include "parser.h"

namespace joy {



	parser::parser(joy_stack& stack, io_device& io, std::string path_to_manual) :
		lexer(stack, io, path_to_manual)
	{
		state_stack.push(state_t::parse);
	}

	parser::state_t parser::state() const {
		return state_stack.top();
	}

	joy_stack parser::tokenize(joy_stack&& tokens) {
		return tokenize_context_free_types(lexer::tokenize(std::move(tokens)));
	}

	bool parser::root_parse(joy_stack& tokens) {
		return(parse(tokens, root_stack));
	}

	bool parser::parse(joy_stack& P, joy_stack& S) {
		for (auto& token : P) {
			if (!parse(token, S)) {
				return false;
			}
		}
		return true;
	}

	bool parser::parse(token_t& token, joy_stack& S) {
		switch (state_stack.top()) {
		case state_t::parse:
			switch (token.second) {
			case joy::joy_t::undef_t:
				return false;
			case joy::joy_t::cmd_t:
				return exec_context_free(token, S) || exec_regular(token, S);
			default:
				S.push(token);
				return true;
			}
		case state_t::list:
		case state_t::quote:
			if (is_sigil(token, "[", "]")) {
				exec_context_free(token, root_stack);
				break;
			}
			nest_token(token, S, root_type, list_depth);
			return true;
		case state_t::set:
			// TODO:
			return false;
			break;
		default:
			throw std::runtime_error("unrecognized state " + to_string(state_stack.top()));
			break;
		}
	}

	void parser::no_conversion(joy_stack& tokens) {
		while (list_depth) {
			state_stack.pop();
			--list_depth;
		}
		lexer::no_conversion(tokens);
	}

	std::string parser::to_string(const state_t match) {
		assert(state_to_string.count(match));
		return state_to_string[match];
	}

	std::string parser::to_colour(const state_t match) {
		assert(state_to_colour.count(match));
		return state_to_colour[match];
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

	bool parser::exec_context_free(token_t& token, joy_stack& S) {
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

	bool parser::is_sigil(token_t& token, std::string&& open_sigil, std::string&& close_sigil) {
		if (jcmd(token)) {
			auto match = std::any_cast<std::string>(token.first);
			return (match == open_sigil || match == close_sigil);
		}
		else {
			return false;
		}
	}

}
