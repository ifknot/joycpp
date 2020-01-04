#pragma once

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

		token_list_t split_quotes(token_list_t& tokens);

		token_list_t split_whitespace(token_list_t& tokens);

	};

}
