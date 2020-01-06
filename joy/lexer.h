/**
* Chomsky type 3 regular grammar lexer 
* Uses a map of string operator to lamda function mapping for Joy operators
* Cascades down to then test for simple types terminating in unrecognised input error
*/
#pragma once

#include "tokenizer.h"
#include "joy_stack.h"
#include "error_messages.h"


namespace joy {

	class lexer : public tokenizer {

	public:

		lexer(joy_stack& stack, io_device& io, std::string path_to_manual);

		void lex(std::string line);

		bool is_quit();

		void quit();

	protected:

		joy_stack& s;	

		bool try_regular(const token_t& token);

		void error(error_number_t e, std::string msg = "");

	private:

		bool no_conversion(const token_t& token);

		bool quit_{ false };

		dictionary_t regular_translation { 
		{"quit", [&]() { quit(); io << ". . ."; }}
		};

	};

}