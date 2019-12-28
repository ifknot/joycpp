#include "parser.h"

namespace joy {

	

	bool parser::tokenize(const std::string& lexeme) {
		//if string and lexeme[size()] == " then close "
		if (try_parse(lexeme)) {
			return true;
		}
		else {
			state_stack.push(state_t::regular_s);
			return lexer::tokenize(lexeme);
		}
	}

	bool parser::try_parse(const std::string& lexeme) {
		return false;
	}

}