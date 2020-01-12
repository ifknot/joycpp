/**
* Parse Joy03 into joy commands and the pass them down to the C++ parser and lexer
*/
#pragma once

#include "parser.h"

namespace joy {

	class parse_joy : public parser {

		using joy_dictionary_t = std::map<std::string, std::string>;

	public:

		parse_joy(joy_stack& stack, io_device& io, std::string path_to_manual);

		void parse(std::string line);

	private:

		bool parse(token_t& token);

		joy_dictionary_t joy_translation {

		};

	};

}
