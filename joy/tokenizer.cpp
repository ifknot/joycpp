#include "tokenizer.h"

namespace joy {

	tokenizer::tokenizer(io_device& io) : io(io) {}

	token_list_t tokenizer::tokenize(std::string line) {
		token_list_t tokens;
		tokens.push_back(make_token(line, joy_t::undef_t));
		size_t size{0};
		while(size < tokens.size()) { // scan for all quoted tokens
			size = tokens.size();
			tokens = split_quotes(tokens);
		} 
		//tokens = split_whitespace(tokens);
		return tokens;
	}

	token_list_t tokenizer::split_quotes(token_list_t& tokens) {
		token_list_t result;
		auto delim = '\"';
		for (const auto& t : tokens) {
			if (t.second == joy_t::undef_t) {
				auto s = std::any_cast<std::string>(t.first);
				auto i = s.find(delim);
				if (i < s.size()) {
					auto j = s.find(delim, i + 1);
					if (j < s.size()) {
						if ((i == 0) && (j == s.size() - 1)) { // "a"
							result.push_back(make_token(s.substr(i + 1, j - 1), joy_t::string_t));
						}
						if ((i == 0) && (j < s.size() - 1)) { // a "b"
							result.push_back(make_token(s.substr(i + 1, j - i - 1), joy_t::string_t));
							result.push_back(make_token(s.substr(j + 1, s.size() - i - 1), joy_t::undef_t));
						}
						if ((i > 0) && (j == s.size() - 1)) { // "a" b
							result.push_back(make_token(s.substr(0, i - 1), joy_t::undef_t));
							result.push_back(make_token(s.substr(i + 1, j - i - 1), joy_t::string_t));
						}
						if ((i > 0) && (j < s.size() - 1)) { // a "b" c
							result.push_back(make_token(s.substr(0, i - 1), joy_t::undef_t));
							result.push_back(make_token(s.substr(i + 1, j - i - 1), joy_t::string_t));
							result.push_back(make_token(s.substr(j + 1, s.size() - i - 1), joy_t::undef_t));
						}
					}
					else {
						result.push_back(t);
					}
				}
				else {
					result.push_back(t);
				}
			}
			else {
				result.push_back(t);
			}
		}
		return result;
	}



}
