#include "parser.h"

namespace joy {



	parser::parser(joy_stack& stack, io_device& io, std::string path_to_manual) :
		lexer(stack, io, path_to_manual)
	{
		state_stack.push(state_t::parse);
	}

	void parser::parse(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		for (auto& t : tokens) {
			if (!parse(t)) {
				break;
			}
		}
	}

	bool parser::parse(token_t& token) {
		switch (state_stack.top()) {
		case joy::state_t::parse:
			if (token.second == joy_t::undef_t) {
				if (try_context_free(token)) {
					return true;
				}
				else {
					return lexer::lex(token);
				}
			}
			s.push(token);
			return true;
		case joy::state_t::list:
		case joy::state_t::quote:
			if (try_context_free(token)) {
				return true;
			}
			if (token.second == joy_t::undef_t) {
				if (!is_regular(token)) {
					unwind();
					return lexer::no_conversion(token);
				}
				token.second = joy_t::cmd_t;
				modify = true;
			}
			nest_token(token, s, list_depth);
			return true;
		case joy::state_t::set:
			break;
		}
	}

	bool parser::try_context_free(token_t& token) {
		if (token.second == joy_t::undef_t) {
			auto it = context_free_translation.find(std::any_cast<std::string>(token.first));
			if (it != context_free_translation.end()) {
				(it->second)();
				return true;
			}
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

	void parser::nest_modify(joy_stack& stack,size_t depth) {
		if (depth == 0) {
			assert(stack.top().second == joy_t::list_t);
			stack.top().second = joy_t::quote_t;
			modify = false;
		}
		else {
			nest_modify(std::any_cast<joy_stack&>(stack.top().first), depth - 1);
		}
	}

	void parser::unwind() {
		while (list_depth--) {
			state_stack.pop();
		}
	}

}
