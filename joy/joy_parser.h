/**
* Parse Joy03 into joy commands and the pass them down to the C++ parser and lexer
*/
#pragma once

#include "parser.h"

namespace joy {

	class joy_parser : public parser {

		using joy_dictionary_t = std::map<std::string, std::string>;

	public:

		joy_parser(joy_stack& stack, io_device& io, std::string path_to_manual);

		void parse(std::string line);

	private:

		void parse_joy(joy_stack& P);

		bool parse_joy(token_t& token);

		bool user_token(token_t& token);

		bool joy_token(token_t& token);

		token_t candidate;

		joy_dictionary_t joy_core {
			{"bar", "manual"}
		};

		joy_dictionary_t joy_user {
			{"foo", "manual"}
		};

	};

}
