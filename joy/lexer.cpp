#include "lexer.h"

namespace joy {

	lexer::lexer(joy_stack& stack, io_device& io, std::string path_to_manual) :
		tokenizer(io),
		root_stack(stack)
	{
		load_manual(path_to_manual);
	}

	bool lexer::is_quit() {
		return quit_;
	}

	void lexer::quit() {
		quit_ = true;
	}

	bool lexer::regular(token_t& token, joy_stack& S) {
		return try_regular(token, S);
	}

	bool lexer::is_regular(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = regular_translation.find(std::any_cast<std::string>(token.first));
		return it != regular_translation.end();
	}

	bool lexer::try_regular(token_t& token, joy_stack& S) {
		auto it = regular_translation.find(std::any_cast<std::string>(token.first));
		if (it != regular_translation.end()) {
			(it->second)(S);
			return true;
		}
		return no_conversion(token);
	}

	bool lexer::no_conversion(token_t& token) {
		return error(XNOCONVERSION, "command lookup >" + to_string(token) + "< " + to_string(token.second) + " is not recognised");
	}

	//helper member functions

	void lexer::load_manual(std::string& path_to_manual) {
		std::ifstream f(path_to_manual);
		if (!f) {
			error(XNOFILE, "load_manual " + path_to_manual + " not found");
		}
		else {
			std::string line, cmd, msg;
			while (std::getline(f, line)) {
				if (line[0] != '#') {
					cmd = line.substr(0, line.find(" "));
					msg = line.substr(line.find(":"), line.size());
					std::getline(f, line);
					joy_manual[cmd] = msg + "\n" + line;
				}
			}
		}
	}

}