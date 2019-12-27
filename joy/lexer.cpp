#include "lexer.h"

namespace joy {



	lexer::lexer(joy_stack& js, io_device& io, std::string path_to_manual) :
		js(js),
		io(io)
	{
		//load_manual(path_to_manual);
	}

	bool lexer::operator()(std::string&& line) {
		std::stringstream line_stream(line);
		std::string s;
		while (line_stream >> s) {
			if (try_regular(s)) {
				//log
			}
			else {
				return no_conversion(s);
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

	bool lexer::try_regular(std::string lexeme) {
		auto it = regular_translation.find(lexeme);
		if (it != regular_translation.end()) {
			(it->second)();
			return true;
		}
		else {
			return try_string(lexeme);
		}
	}

	bool lexer::try_string(std::string lexeme) {
		if (is_joy_string(lexeme)) {
			js.push(make_token(std::move(lexeme), joy_t::string_t));
			return true;
		}
		return try_int(lexeme);
	}

	bool lexer::try_int(std::string lexeme) {
		return false;
	}

	bool lexer::try_char(std::string lexeme) {
		return false;
	}

	bool lexer::try_double(std::string lexeme) {
		return false;
	}

	bool lexer::no_conversion(std::string lexeme) {
		err(DNOCONVERSION);
		return false;
	}

	void lexer::err(error_number_t e, std::string msg) {
		io.colour(RED);
		io << (ERR + std::to_string(e) + " : " + debug_messages[e] + " " +  msg);
		io.colour(BOLDWHITE);
	}

	void lexer::man(std::string command) {
	}

	bool lexer::expects(size_t argc, joy_t argt) {
		for (size_t i{ 0 }; i < argc; ++argc) {
			if (argt != js.sat(i).second) {
				err(DWRONGTYPE, to_string(argt));
				return false;
			}
		}
		return true; 
	}

	void lexer::stack_dump() {
		io.colour(GREEN);
		std::string dump{ "_\n" };
		for (size_t i{ 0 }; i < js.size(); ++i) {
			const auto& [pattern, joy_type] = js.sat(i);
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

	

}
