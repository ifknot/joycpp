#include "lexer.h"

namespace joy {



	lexer::lexer(joy_stack& js, io_device& io) :
		js(js),
		io(io)
	{}

	bool lexer::operator()(std::string&& line) {
		std::stringstream line_stream(line);
		std::string s;
		while (line_stream >> s) {
			if (try_regular(s)) {
				//log
			}
			else {
				return false;
			}
		}
		return true;
	}

	bool lexer::is_exit() {
		return exit_;
	}

	void lexer::exit() {
		exit_ = true;
	}

	bool lexer::try_regular(std::string s) {
		return false;
	}

	

}
