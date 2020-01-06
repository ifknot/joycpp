#include "lexer.h"

namespace joy {



	lexer::lexer(joy_stack& stack, io_device& io, std::string path_to_manual) :
		tokenizer(io),
		s(stack)
	{
		//load_manual(path_to_manual);
	}

	void lexer::lex(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		for (const auto& t : tokens) {
			if (!try_regular(t)) {
				break;
			}
		}
	}

	bool lexer::is_quit() {
		return quit_;
	}

	void lexer::quit() {
		quit_ = true;
	}

	bool lexer::try_regular(const token_t& token) {
		if (token.second == joy_t::undef_t) {			
			auto it = regular_translation.find(std::any_cast<std::string>(token.first));
			if (it != regular_translation.end()) {
				(it->second)();
				return true;
			}
		}
		return no_conversion(token);
	}

	void lexer::error(error_number_t e, std::string msg) {
		io.colour(RED);
		io << (ERR + std::to_string(e) + " : " + error_messages[e] + " " + msg);
		io.colour(BOLDWHITE);
	}

	bool lexer::no_conversion(const token_t& token) {
		error(ENOCONVERSION, "< " + to_string(token) + " > is not recognised");
		return false;
	}

}