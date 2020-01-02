#include "lexer.h"

namespace joy {



	lexer::lexer(joy_stack& stack0, io_device& io, std::string path_to_manual) :
		s0(stack0),
		io(io)
	{
		load_manual(path_to_manual);
	}

	bool lexer::is_exit() {
		return exit_;
	}

	void lexer::exit() {
		exit_ = true;
	}

	bool lexer::tokenize(const std::string& lexeme) {
		return try_regular(lexeme);
	}

	bool lexer::try_regular(const std::string& lexeme) {
		TRACE << __FUNCTION__ << lexeme << "\n";
		auto it = regular_translation.find(lexeme);
		if (it != regular_translation.end()) {
			(it->second)();
			return true;
		}
		else {
			return try_int(lexeme);
		}
	}

	bool lexer::try_int(const std::string& lexeme) { 
		TRACE << __FUNCTION__ << lexeme << "\n";
		if (is_joy_int(lexeme)) {
			s0.push(make_token(lexeme, joy_t::int_t));
			return true;
		}
		return try_char(lexeme);
	}

	bool lexer::try_char(const std::string& lexeme) {
		TRACE << __FUNCTION__ << lexeme << "\n";
		if (is_joy_char(lexeme)) {
			s0.push(make_token(lexeme, joy_t::char_t));
			return true;
		}
		return try_double(lexeme);
	}

	bool lexer::try_double(const std::string& lexeme) {
		TRACE << __FUNCTION__ << lexeme << "\n";
		if (is_joy_double(lexeme)) {
			s0.push(make_token(lexeme, joy_t::double_t));
			return true;
		}
		return no_conversion(lexeme);
	}

	bool lexer::no_conversion(const std::string& lexeme) {
		TRACE << __FUNCTION__ << lexeme << "\n";
		error(DNOCONVERSION, "\"" +lexeme + "\" is not recognised");
		return false;
	}

	void lexer::bin_op_result(token_t& token, joy_stack& stack) {
#ifdef NSUPRESS
		if (stack.sat(1).second < token.second) {
			warn(DPRECISION, "converting " + to_string(token.second) + " to " + to_string(stack.sat(1).second));
		}
#endif
	}

	void lexer::error(error_number_t e, std::string msg) {
		io.colour(RED);
		io << (ERR + std::to_string(e) + " : " + debug_messages[e] + " " + msg);
		io.colour(BOLDWHITE);
	}

	void lexer::warn(error_number_t e, std::string msg) {
		io.colour(YELLOW);
		io << ("warning : " + debug_messages[e] + " " + msg);
		io.colour(BOLDWHITE);
	}

	bool lexer::conforms(const std::initializer_list<joy_t>& argt, const joy_stack& jstack) {
		if (jstack.size() >= argt.size()) {
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
					if (jstack.sat(i).second != t) {
						error(DWRONGTYPE, "stack[" + std::to_string(i) + "] expected: " + to_string(t) + " found: " + to_string(jstack.sat(i).second));
						return false;
					}
					break;
				case joy::joy_t::number_t:
					if ((jstack.sat(i).second == joy_t::int_t) || (jstack.sat(i).second == joy_t::double_t) || (jstack.sat(i).second == joy_t::char_t)) {
						break;
					}
					else {
						error(DWRONGTYPE, "stack[" + std::to_string(i) + "] expected: " + to_string(t) + " found: " + to_string(jstack.sat(i).second));
						return false;
					}
					return false;
				case joy::joy_t::aggregate_t:
					if ((jstack.sat(i).second == joy_t::list_t) || (jstack.sat(i).second == joy_t::quote_t) || (jstack.sat(i).second == joy_t::set_t) || (jstack.sat(i).second == joy_t::string_t)) {
						break;
					}
					else {
						error(DWRONGTYPE, "stack[" + std::to_string(i) + "] expected: " + to_string(t) + " found: " + to_string(jstack.sat(i).second));
						return false;
					}
					return false;
				case joy::joy_t::lexeme_t:
				case joy::joy_t::any_t:
					break;
				case joy::joy_t::undef_t:
				default:
					error(DNOCONVERSION, "stack[" + std::to_string(i) + "] expected: " + to_string(t) + " found: " + to_string(jstack.sat(i).second));
					return false;
				}
				++i;
			}
			return true;
		}
		else {
			error(DLESSARGS, "expected: " + std::to_string(argt.size()) + " found: " + std::to_string(jstack.size()));
			return false;
		}
	}

	void lexer::load_manual(std::string path) {
		std::ifstream f(path);
		if (!f) {
			error(DFILENOTFOUND, "Joy manual " + path);
		}
		else {
			std::string line, cmd, msg;
			std::getline(f, line);
			about_info += line;
			while (std::getline(f, line)) {
				if (line[0] != TOK_CHAR_COMMENT) {
					cmd = line.substr(0, line.find(" "));
					msg = line.substr(line.find(":"), line.size());
					std::getline(f, line);
					joy_manual[cmd] = msg + "\n" + line;
				}
			}
		}
	}

	bool lexer::is_regular(const std::string& lexeme) {
		auto it = regular_translation.find(lexeme);
		return it != regular_translation.end(); 
	}

	void lexer::stack_dump() {
		io.colour(GREEN);
		std::string dump{ "_\n" };
		for (size_t i{ 0 }; i < s0.size(); ++i) {
			const auto& [pattern, joy_type] = s0.sat(i);
			dump += pattern + " " + to_string(joy_type) + "\n";
		}
		io << dump;
		io.colour(BOLDWHITE);
	}

	void lexer::command_dump() {
		io.colour(YELLOW);
		std::string dump;
		for (const auto& [pattern, f] : regular_translation) {
			dump += " " + pattern + " ";
		}
		io << dump;
		io.colour(BOLDWHITE);
	} 

	void lexer::man(std::string match) {
		io.colour(YELLOW);
		auto it = joy_manual.find(match);
		if (it != joy_manual.end()) {
			io << match + joy_manual[match];
		}
		else {
			error(DMANNOTFOUND, s0.top().first);
		}
	}

	void lexer::manual() {
		io.colour(YELLOW);
		for (const auto& [cmd, info] : joy_manual) {
			io << cmd << info;
		}
	}

}
