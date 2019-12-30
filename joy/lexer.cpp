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
		return false;
	}

	bool lexer::no_conversion(const std::string& lexeme) {
		TRACE << __FUNCTION__ << lexeme << "\n";
		err(DNOCONVERSION, lexeme);
		return false;
	}

	void lexer::err(error_number_t e, std::string msg) {
		io.colour(RED);
		io << (ERR + std::to_string(e) + " : " + debug_messages[e] + " " + msg);
		io.colour(BOLDWHITE);
	}

	void lexer::load_manual(std::string path) {
		std::ifstream f(path);
		if (!f) {
			err(DFILENOTFOUND, "Joy manual " + path);
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

	bool lexer::args(size_t n) {
		if (s0.size() == 0) {
			err(DSTACKEMPTY);
			return false;
		}
		else if (s0.size() >= n) {
			return true;
		}
		else {
			err(DLESSARGS, "expected " + std::to_string(n) + " found " + std::to_string(s0.size()));
			return false;
		}
	}

	bool lexer::expects(size_t argc, joy_t argt) {
		for (size_t i{ 0 }; i < argc; ++i) {
			if (args(1)) {
				if (argt != s0.sat(i).second) {
					err(DWRONGTYPE, "expected " + to_string(argt) + " found " + to_string(s0.sat(i).second));
					return false;
				}
			}
			else {
				return false;
			}
		}
		return true;
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
			err(DMANNOTFOUND, s0.top().first);
		}
	}

	void lexer::manual() {
		io.colour(YELLOW);
		for (const auto& [cmd, info] : joy_manual) {
			io << cmd << info;
		}
	}

	void lexer::add() {
	}

	void lexer::subtract() {

	}

	void lexer::multiply() {

	}

	void lexer::divided() {

	}


}
