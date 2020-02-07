#include "parse_context_free.h"

namespace joy {



	parse_context_free::parse_context_free(joy_stack& stack, io_device& io, std::string path_to_manual) :
		parse_regular(stack, io, path_to_manual)
	{
		state_stack.push(state_t::parse);
	}

	parse_context_free::state_t parse_context_free::state() const {
		return state_stack.top();
	}

	joy_stack parse_context_free::tokenize(joy_stack& tokens) {
		return tokenize_context_free_types(parse_regular::tokenize(tokens));
	}

	bool parse_context_free::parse(joy_stack& P, joy_stack& S) {
		for (auto& token : P) {
			if (!parse(token, S)) {
				no_conversion(P);
				return false;
			}
		}
		return true;
	}

	std::string parse_context_free::help() {
		std::string result;
		for (const auto& [key, lamda] : context_free_lambda_map) {
			result += key + " ";
		}
		return result + parse_regular::help();
	}

	bool parse_context_free::call(token_t& token, joy_stack& S) {
		auto it = context_free_lambda_map.find(joy_cast<std::string>(token));
		if (it != context_free_lambda_map.end()) {
			(it->second)(S);
			return true;
		}
		return parse_regular::call(token, S);
	}

	bool parse_context_free::parse(token_t& token, joy_stack& S) {
		switch (state_stack.top()) {
		case state_t::parse:
			switch (token.second) {
			case joy_t::undef_t:
				return false;
			case joy_t::lambda_t:
				return call(token, S);
			default:
				S.push(token);
				return true;
			}
		case state_t::list:
		case state_t::quote:
			if (token == "[" || token == "]") {
				call(token, root_stack);
				break;
			}
			nest_token(token, S, root_type, list_depth);
			return true;
		case state_t::set:
			// TODO: implement Joy sets
			return false;
			break;
		default:
			throw std::runtime_error("unrecognized state " + state_to_string(state_stack.top()));
			return false; 
		}
	}

	void parse_context_free::no_conversion(joy_stack& tokens) {
		while (list_depth) {
			state_stack.pop();
			--list_depth;
		}
		parse_regular::no_conversion(tokens);
	}

	

	std::string parse_context_free::state_to_string(const state_t match) {
		assert(state_map_string.count(match));
		return state_map_string[match];
	}

	std::string parse_context_free::state_to_colour(const state_t match) {
		assert(state_map_colour.count(match));
		return state_map_colour[match];
	}

	joy_stack parse_context_free::tokenize_context_free_types(joy_stack&& tokens) {
		for (auto& [pattern, type] : tokens) {
			if (type == joy_t::undef_t) {
				auto match = std::any_cast<std::string>(pattern);
				auto it = context_free_lambda_map.find(match);
				if (it != context_free_lambda_map.end()) {
					type = joy_t::lambda_t;
				}
			}
		}
		return std::move(tokens);
	}

	void parse_context_free::nest_list(joy_stack& S, size_t depth) {
		if (depth == 0) {
			S.emplace_back(joy_stack{}, joy_t::list_t); //default to a list type...
		}
		else {
			assert(jgroup(S.top()));
			nest_list(joy_cast<joy_stack&>(S.top()), depth - 1);
		}
	}

	void parse_context_free::nest_token(token_t& token, joy_stack& S, joy_t& type, size_t depth) {
		if (depth == 0) {
			S.push(token);
			if (token.second == joy_t::lambda_t) {
				type = joy_t::quote_t; //...unless a joy command is added
				state_stack.pop();
				state_stack.push(state_t::quote);
			}
		}
		else {
			assert(jgroup(S.top())); 
			nest_token(token, joy_cast<joy_stack&>(S.top()), S.top().second, depth - 1);
		}
	}

}
