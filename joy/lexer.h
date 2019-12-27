#pragma once

#include <sstream>

#include "joy_stack.h"
#include "io_device.h"

namespace joy {

	class lexer {

	public:

		lexer(joy_stack& js, io_device& io);

		virtual bool operator()(std::string&& line);

		bool is_exit();

		void exit();

	protected:

		bool try_regular(std::string s);

		joy_stack& js;
		io_device& io;

	private:

		virtual void err(error_number_t e);

		bool exit_{ false };

		/**
		* translate Joy regular grammar commands into their c++ lambda equivalents
		* only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		cpp_dictionary_t regular_translation {
		{TOK_QUIT, [&]() { exit(); io << ". . ."; }},
		};

	};

}

