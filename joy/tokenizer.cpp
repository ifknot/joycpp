#include "tokenizer.h"

namespace joy {

	tokenizer::tokenizer(io_device& io) : io(io) {}

	token_list_t tokenizer::tokenize(std::string line) {
		token_list_t tokens;
		tokens.push_back(make_token(line, joy_t::undef_t)); //entire line as a single undef token
		tokens = split_quotes(tokens); //split out all the open-close quote sections into string tokens 
		tokens = split_whitespace(tokens); //split remaining undef tokens up into sub tokens by white space
		//tokenize any simple types in the fully split list
		// FIX: unable to enter a space character
		for (auto& [pattern, type] : tokens) {
			auto match = std::any_cast<std::string>(pattern);
			if (is_joy_bool(match)) {
				pattern = (match == "true") ? true : false;
				type = joy_t::bool_t;
				continue;
			}
			if (is_joy_char(match)) {
				pattern = match[1];
				type = joy_t::char_t;
				continue;
			}
			if (is_joy_int(match)) {
				pattern = stoi(match);
				type = joy_t::int_t;
				continue;
			}
			if (is_joy_double(match)) {
				pattern = stod(match);
				type = joy_t::double_t;
				continue;
			}
		}
		return tokens;
	}

	token_list_t tokenizer::split_quotes(token_list_t& tokens) {
		token_list_t result;
		for (const auto& token : tokens) {  //examine all the tokens
			rec_sigil_split(token, result, '\"', '\"'); //recursively find and split out open-close quotes into tokens
		}
		return result;
	}

	void tokenizer::rec_sigil_split(token_t token, token_list_t& tokens, char open_sigil, char close_sigil) {
		if (token.second == joy_t::undef_t) {
			auto lexeme = std::any_cast<std::string>(token.first);
			auto i = lexeme.find(open_sigil);
			if (i < lexeme.size()) { //found an opening sigil
				auto j = lexeme.find(close_sigil, i + 1);
				if (j < lexeme.size()) { //found a closing sigil
					//split the token up into first sigil enclosed section (s) and any undefs (a) before (s) and recurse on any undefs (b) after (s)
					if ((i == 0) && (j == lexeme.size() - 1)) { // "s"
						tokens.push_back(make_token(lexeme.substr(i + 1, j - i - 1), joy_t::string_t));
					}
					if ((i == 0) && (j < lexeme.size() - 1)) { // "s" b
						tokens.push_back(make_token(lexeme.substr(1, j - i - 1), joy_t::string_t));
						//recursively examine the b section after the closing sigil
						rec_sigil_split(make_token(lexeme.substr(j + 1, lexeme.size() - j + 1), joy_t::undef_t), tokens, '\"', '\"');
					}
					if ((i > 0) && (j == lexeme.size() - 1)) { // a "s"
						tokens.push_back(make_token(lexeme.substr(0, i - 1), joy_t::undef_t));
						tokens.push_back(make_token(lexeme.substr(i + 1, j - i - 1), joy_t::string_t));
					}
					if ((i > 0) && (j < lexeme.size() - 1)) { // a "s" b
						tokens.push_back(make_token(lexeme.substr(0, i - 1), joy_t::undef_t));
						tokens.push_back(make_token(lexeme.substr(i + 1, j - i - 1), joy_t::string_t));
						//recursively examine the b section after the closing sigil
						rec_sigil_split(make_token(lexeme.substr(j + 1, lexeme.size() - j + 1), joy_t::undef_t), tokens, '\"', '\"');
					}
				}
				else { //no closing sigil push unmodified token
					tokens.push_back(token);
				}
			}
			else { //no opening sigil push unmodified token
				tokens.push_back(token);
			}
		}
		else { //not an undef so push unmodified token
			tokens.push_back(token);
		}
	}

	token_list_t tokenizer::split_whitespace(token_list_t& tokens) {
		token_list_t result;
		for (const auto& t : tokens) { //examine all the tokens
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