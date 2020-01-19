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

	bool parser::stack_parse(joy_stack& tokens, joy_stack& S) {
		for (auto token : tokens) {
			switch (state_stack.top()) {
			case joy::state_t::comment:
				return true;
			case joy::state_t::parse:
				if (!token_parse(token, S) && !lexer::token_parse(token, S)) {
					return false;
				}
				break;
			case joy::state_t::quote:
			case joy::state_t::list: {
				if(list_sigil(token)) {
					run(token, S);
					break;
				}
				nest_token(token, S, root_type, list_depth);
				break;
			}
			case joy::state_t::set:
				break;
			default:
				throw std::runtime_error("unrecognised state " + to_string(state_stack.top()));
				break;
			}
		}
		return true;
	}

	bool parser::token_parse(token_t& token, joy_stack& S) {
		switch (token.second) {
		case joy::joy_t::undef_t:
			return false;
		case joy::joy_t::cmd_t:
			return run(token, S);
		default:
			return false;
		}
	}

	void parser::no_conversion(joy_stack& tokens) {
		while (list_depth) {
			state_stack.pop();
			--list_depth;
		}
		lexer::no_conversion(tokens);
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

}
