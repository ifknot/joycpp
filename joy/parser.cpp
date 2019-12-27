#include "parser.h"

namespace joy {

	

	bool parser::tokenize(const std::string& lexeme) {
		return lexer::tokenize(lexeme);
	}

}