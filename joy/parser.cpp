#include "parser.h"

namespace joy {



	parser::parser(joy_stack& stack, io_device& io, std::string path_to_manual) :
		lexer(stack, io, path_to_manual)
	{
		state_stack.push(state_t::parse);
	}

	void parser::parse(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		parse(tokens, root_stack);
	}

	void parser::operator()(joy_stack& P, joy_stack& S) {
		parse(P, S);
	}

	void parser::operator()(joy_stack&& P, joy_stack& S) {
		parse(P, S);
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
		assert(state_stack.top() == state_t::parse);
		for (auto& token : P) {
			if (!parse(token, S)) {
				break;
			}
		}
	}

	bool parser::parse(token_t& token, joy_stack& S) {
		if (jundef(token)) {
			if (state_change(token, S)) {
				return true;
			}
			if (is_context_free(token) || is_regular(token)) {
				token.second = joy_t::cmd_t;
			}
			else {
				unwind(S);
				return lexer::no_conversion(token);
			}
		}
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
			break;
		}
	}

	bool parser::is_context_free(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = context_free_atoms.find(std::any_cast<std::string>(token.first));
		if (it != context_free_atoms.end()) {
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
			}
		}
		else {
			assert(jgroup(S.top())); 
			nest_token(token, std::any_cast<joy_stack&>(S.top().first), S.top().second, depth - 1);
		}
	}

	void parser::unwind(joy_stack& S) {
		while (list_depth) {
			//if S.size { //see whats going on...
			// 	io << S.top();
			//	S.pop();
			//}
			--list_depth;
		}
	}

}
