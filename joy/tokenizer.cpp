#include "tokenizer.h"

namespace joy {

	tokenizer::tokenizer(io_device& io) : io(io) {}

	token_list_t tokenizer::tokenize(std::string line) {
		token_list_t tokens;
		tokens.push_back(make_token(line, joy_t::undef_t));
		size_t size{0};
		while(size < tokens.size()) { // split quoted into tokens
			size = tokens.size();
			tokens = split_quotes(tokens);
		} 
		tokens = split_whitespace(tokens); //split up tokens containing white space
		return tokens;
	}

	token_list_t tokenizer::split_quotes(token_list_t& tokens) {
		token_list_t result;
		auto delim = '\"';
		for (const auto& token : tokens) {
			if (token.second == joy_t::undef_t) { //check only undef tokens for quotes
				auto lexeme = std::any_cast<std::string>(token.first);
				auto i = lexeme.find(delim);
				if (i < lexeme.size()) {
					auto j = lexeme.find(delim, i + 1);
					if (j < lexeme.size()) { //split the token up string and any undefs
						if ((i == 0) && (j == lexeme.size() - 1)) { // "a"
							result.push_back(make_token(lexeme.substr(i + 1, j - 1), joy_t::string_t));
						}
						if ((i == 0) && (j < lexeme.size() - 1)) { // a "b"
							result.push_back(make_token(lexeme.substr(i + 1, j - i - 1), joy_t::string_t));
							result.push_back(make_token(lexeme.substr(j + 1, lexeme.size() - i - 1), joy_t::undef_t));
						}
						if ((i > 0) && (j == lexeme.size() - 1)) { // "a" b
							result.push_back(make_token(lexeme.substr(0, i - 1), joy_t::undef_t));
							result.push_back(make_token(lexeme.substr(i + 1, j - i - 1), joy_t::string_t));
						}
						if ((i > 0) && (j < lexeme.size() - 1)) { // a "b" c
							result.push_back(make_token(lexeme.substr(0, i - 1), joy_t::undef_t));
							result.push_back(make_token(lexeme.substr(i + 1, j - i - 1), joy_t::string_t));
							result.push_back(make_token(lexeme.substr(j + 1, lexeme.size() - i - 1), joy_t::undef_t));
						}
					}
					else {
						result.push_back(token); 
					}
				}
				else {
					result.push_back(token);
				}
			}
			else {
				result.push_back(token);
			}
		}
		return result;
	}

	token_list_t tokenizer::split_whitespace(token_list_t& tokens) {
		token_list_t result;
		for (const auto& t : tokens) {
			if (t.second == joy_t::undef_t) { //check only undef tokens 
				std::stringstream ss(std::any_cast<std::string>(t.first));
				std::string lexeme;
				while (ss >> lexeme) { //current token has lexemes separated by white space
					result.push_back(make_token(lexeme, joy_t::undef_t));
				}
			}
			else {
				result.push_back(t);
			}
		}
		return result;
	}

}