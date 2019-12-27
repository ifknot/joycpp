#include "lexer.h"

namespace joy {



	lexer::lexer(joy_stack& js, io_device& io) :
		js(js),
		io(io)
	{}

	bool lexer::operator()(std::string&& line) {
		return true;
	}

	bool lexer::is_exit() {
		return exit_;
	}

	void lexer::exit() {
		exit_ = true;
	}

}
