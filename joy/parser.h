#pragma once

#include <stack>

#include "lexer.h"

namespace joy {

	class parser : public lexer {

		enum class state_t {regular_s, quote_s, list_s, set_s, string_s, define_s};

	public:

		using lexer::lexer;

	protected:

		virtual bool tokenize(const std::string& lexeme) override;

		std::stack<state_t> states;

	private:
		
		
	};

}

