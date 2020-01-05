#include "lexer.h"

namespace joy {



	lexer::lexer(joy_stack& stack, io_device& io, std::string path_to_manual) :
		tokenizer(io),
		s(stack)
	{
		//load_manual(path_to_manual);
	}

	bool lexer::is_exit() {
		return exit_;
	}

	void lexer::exit() {
		exit_ = true;
	}

}