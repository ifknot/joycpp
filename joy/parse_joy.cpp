#include "parse_joy.h"

namespace joy {

	parse_joy::parse_joy(joy_stack& stack, io_device& io, std::string path_to_manual) :
		parser(stack, io, path_to_manual)
	{}

	void parse_joy::parse(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		for (auto& token : tokens) {
			if (!parse(token)) {
				break;
			}
		}
	}
	bool parse_joy::parse(token_t& token) {
		return false;
	}
}