#include "parser.h"

namespace joy {



	parser::parser(joy_stack& stack, io_device& io, std::string path_to_manual) :
		lexer(stack, io, path_to_manual)
	{
		state_stack.push(state_t::parse);
	}

	void parser::parse(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		parse(tokens);
	}

	void parser::parse(joy_stack& stack) {
		assert(state_stack.top() == state_t::parse);
		for (auto& token : stack) {
			if (!parse(token)) {
				break;
			}
		}
	}

	void parser::parse(joy_stack&& stack) {
		assert(state_stack.top() == state_t::parse);
		for (auto& token : stack) {
			if (!parse(token)) {
				break;
			}
		}
	}

	bool parser::parse(token_t& token) {
		if (jundef(token)) {
			if (try_special(token)) {
				return true;
			}
			if (is_parsable(token) || is_lexable(token)) {
				token.second = joy_t::cmd_t;
			}
			else {
				unwind();
				return lexer::no_conversion(token);
			}
		}
		switch (state_stack.top()) {
		case joy::state_t::parse:
			if (jcmd(token)) {
				// FIX: special || context_free || regular
				return try_special(token) || try_context_free(token) || lexer::lex(token);
			} 
			else {
				root_stack.push(token);
				return true;
			}
		case joy::state_t::list:
		case joy::state_t::quote:
			nest_token(token, root_stack, root_type, list_depth);
			return true;
		case joy::state_t::set:
			// TODO:
			return false;
			break;
		}
	}

	bool parser::is_parsable(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = context_free_translation.find(std::any_cast<std::string>(token.first));
		if (it != context_free_translation.end()) {
			return true;
		}
		return false;
	}

	bool parser::try_special(token_t& token) {
		auto it = special_translation.find(std::any_cast<std::string>(token.first));
		if (it != special_translation.end()) {
			(it->second)();
			return true;
		}
		return false;
	}

	bool parser::try_context_free(token_t& token) {
		auto it = context_free_translation.find(std::any_cast<std::string>(token.first));
		if (it != context_free_translation.end()) {
			(it->second)();
			return true;
		}
		return false;
	}

	void parser::nest_list(joy_stack& stack, size_t depth) {
		if (depth == 0) {
			stack.emplace_back(joy_stack{}, joy_t::list_t); //default to a list type...
		}
		else {
			// FIX: use tsequence
			assert((root_stack.top().second == joy_t::list_t) || (root_stack.top().second == joy_t::quote_t));
			nest_list(std::any_cast<joy_stack&>(stack.top().first), depth - 1);
		}
	}

	void parser::nest_token(token_t& token, joy_stack& stack, joy_t& type, size_t depth) {
		if (depth == 0) {
			stack.push(token);
			if (token.second == joy_t::cmd_t) {
				type = joy_t::quote_t; //...unless a joy command is added
			}
		}
		else {
			// FIX: use tsequence
			assert((root_stack.top().second == joy_t::list_t) || (root_stack.top().second == joy_t::quote_t));
			nest_token(token, std::any_cast<joy_stack&>(stack.top().first), stack.top().second, depth - 1);
		}
	}

	void parser::unwind() {
		while (list_depth) {
			state_stack.pop();
			--list_depth;
		}
	}

	joy_stack parser::step(joy_stack& stack) {
		auto P = std::any_cast<joy_stack&>(stack.top().first);
		auto X = stack.sat(1);
		stack.pop2();
		auto A = std::any_cast<joy_stack&>(X.first);
		joy_stack S;
		for (auto a : A) {
			S.push(a);
			for (auto p : P) {
				S.push(p);
			}
		}
		return S;
	}

	void parser::reverse(joy_stack& stack) {
		auto& A = std::any_cast<joy_stack&>(stack.top().first);
		std::reverse(A.begin(), A.end());
	}

	token_t parser::map(joy_stack& stack) {
		auto size = std::any_cast<joy_stack&>(stack.sat(1).first).size(); //aggregate size
		auto type = stack.sat(1).second; //get the return aggregate type
		parse(step(stack));
		joy_stack S;
		while (size--) {
			S.push(root_stack.top());
			root_stack.pop();
		}
		std::reverse(S.begin(), S.end()); 
		return make_token(S, type);
	}

	void parser::dip(joy_stack& stack) {
		auto P = std::any_cast<joy_stack&>(stack.top().first);
		const auto X = root_stack.sat(1);
		root_stack.pop2();
		parse(P);
		stack.push(X);
	}

	void parser::i(joy_stack& stack) {
		auto P = std::any_cast<joy_stack&>(stack.top().first);
		stack.pop();
		parse(P);
	}

}
