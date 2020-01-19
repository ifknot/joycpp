#include "lexer.h"

namespace joy {

	lexer::lexer(joy_stack& stack, io_device& io, std::string path_to_manual) :
		tokenizer(io),
		root_stack(stack)
	{
		load_manual(path_to_manual);
	}
	  
	joy_stack lexer::tokenize(joy_stack&& tokens) {
		return tokenize_regular_types(tokenizer::tokenize(std::move(tokens)));
	}

	bool lexer::parse(joy_stack& tokens) {
		//bool parse(token)
		for(auto& token : tokens) {
			switch (token.second) {
			case joy::joy_t::undef_t:
				return false;
			case joy::joy_t::cmd_t:
				if (!run(token, root_stack)) {
					return false;
				}
				break;
			case joy::joy_t::string_t:
			case joy::joy_t::bool_t:
			case joy::joy_t::char_t:
			case joy::joy_t::int_t:
			case joy::joy_t::double_t:
				root_stack.push(token);
				break;
			default:
				break;
			}
		}
		return true;
		//
	}

	void lexer::no_conversion(joy_stack& tokens) {
		for (auto& [pattern, type] : tokens) {
			if (type == joy_t::undef_t) {
				auto culprit = std::any_cast<std::string>(pattern);
				error(XNOCONVERSION, "command lookup >" + culprit + "< " + to_string(type) + " is not recognised");
			}
		}
		print_stack(root_stack, io);
	}

	bool lexer::is_quit() {
		return quit_;
	}

	void lexer::quit() {
		quit_ = true;
	}

	bool lexer::regular(token_t& token, joy_stack& S) {
		auto it = regular_atoms.find(std::any_cast<std::string>(token.first));
		if (it != regular_atoms.end()) {
			(it->second)(S);
			return true;
		}
		return false; 
	}

	bool lexer::is_regular(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = regular_atoms.find(std::any_cast<std::string>(token.first));
		return it != regular_atoms.end();
	}

	joy_stack lexer::tokenize_regular_types(joy_stack&& tokens) {
		for (auto& [pattern, type] : tokens) {
			if (type == joy_t::undef_t) {
				auto match = std::any_cast<std::string>(pattern);
				auto it = regular_atoms.find(match);
				if (it != regular_atoms.end()) {
					type = joy_t::cmd_t;
				}
			}
		}
		return std::move(tokens);
	}

	bool lexer::run(token_t& token, joy_stack& S) {
		auto it = regular_atoms.find(std::any_cast<std::string>(token.first));
		if (it != regular_atoms.end()) {
			(it->second)(S);
			return true;
		}
		return false;
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