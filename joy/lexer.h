/**
* Chomsky type 3 regular grammar lexer 
* Uses a map of string operator to lamda function mapping for Joy operators
* Cascades down to then test for simple types terminating in unrecognised input error
*/
#pragma once

#include "tokenizer.h"
#include "joy_stack.h"


namespace joy {

	class lexer : public tokenizer {

	public:

		lexer(joy_stack& stack, io_device& io, std::string path_to_manual);

		bool is_exit();

		void exit();

	protected:

		joy_stack& s;	

	private:

		bool exit_{ false };

	};

}