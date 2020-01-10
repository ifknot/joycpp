#include "lexer.h"

namespace joy {



	lexer::lexer(joy_stack& stack, io_device& io, std::string path_to_manual) :
		tokenizer(io),
		s(stack)
	{
		load_manual(path_to_manual);
	}

	void lexer::lex(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		for (const auto& t : tokens) {
			if (t.second == joy_t::undef_t) {
				if (!lex(t)) {
					break;
				}
			}
			else {
				s.push(t);
			}
		}
	}

	bool lexer::is_quit() {
		return quit_;
	}

	void lexer::quit() {
		quit_ = true;
	}

	bool lexer::lex(token_t token) {
		return try_regular(token);
	}

	void lexer::error(error_number_t e, std::string msg) {
		io.colour(RED);
		io << (ERR + std::to_string(e) + " : " + error_messages[e] + " " + msg);
		io.colour(BOLDWHITE);
	}

	bool lexer::has(const std::initializer_list<joy_t>& argt, const joy_stack& stack) {
		if (stack.size() >= argt.size()) {
			size_t i{ 0 };
			for (const auto& t : argt) { //for each of the joy types in the initializer list
				switch (t) {
				case joy::joy_t::bool_t:
				case joy::joy_t::int_t:
				case joy::joy_t::char_t:
				case joy::joy_t::double_t:
				case joy::joy_t::list_t:
				case joy::joy_t::quote_t:
				case joy::joy_t::set_t:
				case joy::joy_t::string_t:
					if (stack.sat(i).second != t) {
						error(DWRONGTYPE, "stack[" + std::to_string(i) + "] expected: " + to_string(t) + " found: " + to_string(stack.sat(i).second));
						return false;
					}
					break;
				case joy::joy_t::numeric_t:
					if ((stack.sat(i).second == joy_t::int_t) || (stack.sat(i).second == joy_t::double_t) || (stack.sat(i).second == joy_t::char_t)) {
						break;
					}
					else {
						error(DWRONGTYPE, "stack[" + std::to_string(i) + "] expected: " + to_string(t) + " found: " + to_string(stack.sat(i).second));
						return false;
					}
					return false;
				case joy::joy_t::aggregate_t:
					if ((stack.sat(i).second == joy_t::list_t) || (stack.sat(i).second == joy_t::quote_t) || (stack.sat(i).second == joy_t::set_t) || (stack.sat(i).second == joy_t::string_t)) {
						break;
					}
					else {
						error(DWRONGTYPE, "stack[" + std::to_string(i) + "] expected: " + to_string(t) + " found: " + to_string(stack.sat(i).second));
						return false;
					}
					return false;
				case joy::joy_t::any_t:
					break;
				case joy::joy_t::undef_t:
				default:
					error(DNOCONVERSION, "stack[" + std::to_string(i) + "] expected: " + to_string(t) + " found: " + to_string(stack.sat(i).second));
					return false;
				}
				++i;
			}
			return true;
		}
		else {
			error(DLESSARGS, "expected: " + std::to_string(argt.size()) + " found: " + std::to_string(stack.size()));
			return false;
		}
	}

	bool lexer::try_regular(token_t& token) {
		if (token.second == joy_t::undef_t) {
			auto it = regular_translation.find(std::any_cast<std::string>(token.first));
			if (it != regular_translation.end()) {
				(it->second)();
				return true;
			}
		}
		return no_conversion(token);
	}

	bool lexer::no_conversion(token_t& token) {
		error(DNOCONVERSION, "< " + to_string(token) + " > is not recognised");
		return false;
	}

	//helper member functions

	void lexer::print_stack(const joy_stack& stack) {
		io.colour(GREEN);
		std::string dump{ "_\n" };
		for (size_t i{ 0 }; i < stack.size(); ++i) {
			const auto& t = stack.sat(i);
			dump += to_string(t) + "\n";
		}
		io << dump;
	}

	void lexer::load_manual(std::string& path_to_manual) {
		std::ifstream f(path_to_manual);
		if (!f) {
			error(DFILENOTFOUND, "Joy manual " + path_to_manual);
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

	void lexer::manual() {
		io.colour(YELLOW);
		for (const auto& [cmd, info] : joy_manual) {
			io << cmd << info;
		}
	}

	void lexer::helpdetail(std::string match) {
		io.colour(YELLOW);
		auto it = joy_manual.find(match);
		if (it != joy_manual.end()) {
			io << match + joy_manual[match];
		}
	}

}