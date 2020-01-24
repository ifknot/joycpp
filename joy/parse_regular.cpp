#include "parse_regular.h"

namespace joy {

	parse_regular::parse_regular(joy_stack& stack, io_device& io, std::string path_to_manual) :
		tokenizer(io),
		root_stack(stack)
	{
		load_manual(path_to_manual);
		// TODO: reserve root_stack space?
	}
	  
	joy_stack parse_regular::tokenize(joy_stack&& tokens) {
		return tokenize_regular_types(tokenizer::tokenize(std::move(tokens)));
	}

	void parse_regular::no_conversion(joy_stack& tokens) {
		io << "reg no " + std::to_string(tokens.size());
		for (auto& [pattern, type] : tokens) {
			io << joy_stack::to_string(make_token(pattern, type));
			switch (type) {
			case joy::joy_t::undef_t:
			case joy::joy_t::lamda_t:
			case joy::joy_t::joy_t: {
				auto culprit = std::any_cast<std::string>(pattern);
				error(XNOCONVERSION, "command lookup >" + culprit + "< " + to_string(type) + " is not recognized");
				break;
			}
			default:
				break;
			}
		}
	}

	bool parse_regular::is_quit() {
		return quit_;
	}

	void parse_regular::quit() {
		quit_ = true;
	}

	joy_stack parse_regular::tokenize_regular_types(joy_stack&& tokens) {
		for (auto& [pattern, type] : tokens) {
			if (type == joy_t::undef_t) {
				auto match = std::any_cast<std::string>(pattern);
				auto it = regular_lambda_map.find(match);
				if (it != regular_lambda_map.end()) {
					type = joy_t::lamda_t;
				}
			}
		}
		return std::move(tokens);
	}

	bool parse_regular::call(token_t& token, joy_stack& S) {
		io << "reg call";
		auto it = regular_lambda_map.find(std::any_cast<std::string>(token.first));
		if (it != regular_lambda_map.end()) {
			(it->second)(S);
			io << "true";
			return true;
		}
		io << "false";
		return false;
	}


	void parse_regular::load_manual(std::string& path_to_manual) {
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
