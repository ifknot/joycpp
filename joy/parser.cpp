#include "parser.h"

namespace joy {



	parser::parser(joy_stack& stack, io_device& io, std::string path_to_manual) :
		lexer(stack, io, path_to_manual)
	{
		state_stack.push(state_t::parse);
	}

	void parser::parse(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		for (auto& token : tokens) {
			if (!parse(token)) {
				break;
			}
		}
	}

	void parser::parse(joy_stack& stack) {
		assert(state_stack.top() == state_t::parse);
		for (auto& token : stack) {
			if (!parse(token)) {
				break;
			}
		}
	}

	bool parser::parse(token_t& token) {
		switch (state_stack.top()) {
		case joy::state_t::parse:
			if (token.second == joy_t::undef_t) {
				return try_special(token) || try_context_free(token) || lexer::lex(token);
			} 
			s.push(token);
			return true;
		case joy::state_t::list:
		case joy::state_t::quote:
			if (token.second == joy_t::undef_t) {
				if (try_special(token)) {
					return true;
				}
				if (!is_parsable(token) && !is_lexable(token)) {
					unwind();
					return lexer::no_conversion(token);
				}
			}
			nest_token(token, s, list_depth);
			return true;
		case joy::state_t::set:
			break;
		}
	}

	bool parser::is_parsable(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = context_free_translation.find(std::any_cast<std::string>(token.first));
		if (it != context_free_translation.end()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool parser::try_special(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = special_translation.find(std::any_cast<std::string>(token.first));
		if (it != special_translation.end()) {
			(it->second)();
			return true;
		}
		return false;
	}

	bool parser::try_context_free(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = context_free_translation.find(std::any_cast<std::string>(token.first));
		if (it != context_free_translation.end()) {
			(it->second)();
			return true;
		}
		return false;
	}

	void parser::nest_list(joy_stack& stack, size_t depth) {
		if (depth == 0) {
			stack.emplace_back(joy_stack{}, joy_t::list_t);
		}
		else {
			assert(s.top().second == joy_t::list_t);
			nest_list(std::any_cast<joy_stack&>(stack.top().first), depth - 1);
		}
	}

	void parser::nest_token(token_t& token, joy_stack& stack, size_t depth) {
		if (depth == 0) {
			stack.push(token);
		}
		else {
			assert(stack.top().second == joy_t::list_t);
			nest_token(token, std::any_cast<joy_stack&>(stack.top().first), depth - 1);
		}
	}

	void parser::unwind() {
		while (list_depth--) {
			state_stack.pop();
		}
	}

}
