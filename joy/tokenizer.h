#pragma once

#include <sstream>

#include "io_device.h"
#include "joy_types.h"

namespace joy {

	inline token_t make_token(std::any arg, joy_t type) {
		return std::make_pair(arg, type);
	}

	class tokenizer {
	
	public:
	
		tokenizer(io_device& io);

		token_list_t tokenize(std::string line);

	protected:

		io_device& io;

	private:

		/**
		* split out all the open-close quote sections into string tokens 
		*/
		token_list_t split_quotes(token_list_t& tokens);

		/**
		* recursively find and split out sigil enclosed sections into tokens	
		*/
		void rec_sigil_split(token_t token, token_list_t& tokens, char open_sigil, char close_sigil);

		/**
		* split out all the white space separated sections into string tokens 
		*/
		token_list_t split_whitespace(token_list_t& tokens);

	};

}
