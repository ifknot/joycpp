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
		auto it = regular_translation.find(std::any_cast<std::string>(token.first));
		if (it != regular_translation.end()) {
			(it->second)(S);
			return true;
		}
		return false; 
	}

	bool lexer::is_regular(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = regular_translation.find(std::any_cast<std::string>(token.first));
		return it != regular_translation.end();
	}	

	void lexer::load_manual(std::string& path_to_manual) {
		std::ifstream f(path_to_manual);
		if (!f) {
			error(XNOFILE, "load_manual " + path_to_manual + " not found");
		}
		else {
			std::string cmd_name, cmd_algebra, summary;
			while (std::getline(f, summary)) {
				if (summary[0] != '#') {
					cmd_name = summary.substr(0, summary.find(" "));
					cmd_algebra = summary.substr(summary.find(":"), summary.size());
					std::getline(f, summary);
					joy_manual[cmd_name] = cmd_algebra + "\n" + summary;
				}
			}
		}
	}

}