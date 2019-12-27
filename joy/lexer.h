#pragma once

#include "joy_stack.h"
#include "io_device.h"

namespace joy {

	class lexer {

	public:

		lexer(joy_stack& js, io_device& io);

		virtual bool operator()(std::string&& line);

		bool is_exit();

		void exit();

	private:

		joy_stack& js;
		io_device& io;

		bool exit_{ false };

	};

}

