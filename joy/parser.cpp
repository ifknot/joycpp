#include "parser.h"

namespace joy {

	parser::parser(joy_stack& stack0, io_device& io, std::string path_to_manual) : lexer(stack0, io, path_to_manual) {
		state_stack.push(state_t::parse);
	}

	bool parser::operator()(std::string&& line) {
		std::stringstream line_stream(line);
		std::string lexeme;
		while (line_stream >> lexeme) {
			if (tokenize(lexeme)) {
				//log
			}
			else {
				return false;
			}
		}
		return true;
	}

	bool parser::tokenize(const std::string& lexeme) {
		if (!try_parse(lexeme)) { // tidy up s1 and nesting counters
			std::cout << quote_depth;
			while (quote_depth) {
				state_stack.pop();
				--quote_depth;
			}
			s1.newstack();
			io << ("->" + std::to_string(quote_depth) + to_string(state_stack.top()));
			io.colour(to_colour(state_stack.top()));
			return false; 
		}
		else {
			return true;
		}
	}

	bool parser::is_context_free(const std::string& lexeme) {
		auto it = context_free_translation.find(lexeme);
		return it != context_free_translation.end();
	}

	bool parser::try_parse(const std::string& lexeme) {
		switch (state_stack.top())
		{
		case joy::state_t::parse:
			io.colour(to_colour(state_stack.top()));
			TRACE << to_string(state_stack.top());
			return try_context_free(lexeme);
		case joy::state_t::quote:
			return try_build_quote(lexeme);
		case joy::state_t::list:
			break;
		case joy::state_t::set:
			break;
		case joy::state_t::string:
			return try_build_string(lexeme);
		default:
			break;
		}
		return false;
	}

	bool parser::try_context_free(const std::string& lexeme) {
		TRACE << __FUNCTION__ << "\n";
		auto it = context_free_translation.find(lexeme);
		if (it != context_free_translation.end()) {
			(it->second)();
			return true;
		}
		else {
			return try_string(lexeme);
		}
	}

	//string is a special case needing its own try 
	//FIX: doesnt handle whitespace properly strips any thing over one space thanks to using stringstream to split line
	bool parser::try_string(const std::string& lexeme) {
		TRACE << __FUNCTION__ << lexeme << to_string(state_stack.top()) << "\n";
		if (lexeme[0] == STRING_OPEN) {
			state_stack.push(state_t::string);
			io.colour(to_colour(state_stack.top()));
			TRACE << to_string(state_stack.top());
			if ((lexeme.size() > 1) && (lexeme.back() == STRING_CLOSE)) { //string with no spaces
				s0.push(make_token(lexeme.substr(0,lexeme.size()), joy_t::string_t));
				state_stack.pop();
				io.colour(to_colour(state_stack.top()));
				return true;
			}
			else { //spaces in the string
				assert(string_build.size() == 0);
				string_build += lexeme;
				return true;
			}
		}
		else {
			return lexer::tokenize(lexeme);
		}
	}

	bool parser::try_build_quote(const std::string& lexeme) {
		TRACE << __FUNCTION__ << lexeme << to_string(state_stack.top()) << "\n";
		if (lexeme == "[") {
			state_stack.push(state_t::quote);
			io.colour(to_colour(state_stack.top()));
			quote_depth++;
		}
		if (is_context_free(lexeme) || is_regular(lexeme) || (joy_type(lexeme) != joy_t::undef_t)) {			
			s1.push(make_token(lexeme, joy_t::lexeme_t));
			if (lexeme == "]") {
				--quote_depth;
				state_stack.pop();
				if (quote_depth == 0) {
					s1.semi_stack();
					s0.push(make_token(s1.top().first, joy_t::quote_t));
					s1.newstack();
					io.colour(to_colour(state_stack.top()));
				}
			}
			return true;
		}
		else return false;
	}

	bool parser::try_build_string(const std::string& lexeme) {
		TRACE << __FUNCTION__ << lexeme << to_string(state_stack.top()) << "\n";
		assert(state_stack.top() == state_t::string);
		string_build += " " + lexeme;
		if (lexeme.back() == STRING_CLOSE) {
			s0.push(make_token(std::move(string_build), joy_t::string_t));
			string_build = "";
			state_stack.pop();
			io.colour(to_colour(state_stack.top()));
		}
		return true;
	}

	void parser::include() {
		auto path = s0.top().first;
		s0.pop();
		std::ifstream f(destring(path));
		if (!f) {
			error(DFILENOTFOUND, "include " + path);
		}
		else {
			std::string line;
			while (std::getline(f, line)) {
				if (line[0] != TOK_CHAR_COMMENT) {
					operator()(std::move(line));
				}
			}
		}

	}

}