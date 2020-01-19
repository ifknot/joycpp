#include "tokenizer.h"

#include "iostream"

namespace joy {

	tokenizer::tokenizer(io_device& io) : io(io) {}

	joy_stack tokenizer::tokenize(std::string&& line) {
		joy_stack tokens;
		trim(line); //trim leading spaces and any single line comments
		if (line.empty()) { //ignore any empty line 
			return tokens;
		}
		tokens.push_back(make_token(line, joy_t::undef_t)); //entire line as a single undef token
		return tokenize(std::move(tokens)); //split & tokenize 
	}

	joy_stack tokenizer::tokenize(joy_stack&& tokens) {
		//split out all the open-close quote sections into string_t tokens 
		//split remaining undef tokens up into sub tokens by white space
		//tokenize any simple types in the fully split stack of tokens
		return tokenize_simple_types(split_whitespace(tokenize_strings(std::move(tokens))));
	}

	joy_stack tokenizer::tokenize_strings(joy_stack&& tokens) {
		joy_stack result;
		for (const auto& token : tokens) {  //examine all the tokens
			rec_sigil_split(token, result, '\"', '\"', joy_t::string_t); //recursively find and split out open-close quotes into tokens
		}
		return result;
	}

	void tokenizer::rec_sigil_split(token_t token, joy_stack& tokens, char open_sigil, char close_sigil, joy_t sigil_type) {
		if (token.second == joy_t::undef_t) {
			auto lexeme = std::any_cast<std::string>(token.first);
			auto i = lexeme.find(open_sigil);
			if (i < lexeme.size()) { //found an opening sigil
				auto j = lexeme.find(close_sigil, i + 1);
				if (j < lexeme.size()) { //found a closing sigil
					//split the token up into first sigil enclosed section (s) and any undefs (a) before (s) and recurse on any undefs (b) after (s)
					if ((i == 0) && (j == lexeme.size() - 1)) { // "s"
						tokens.push_back(make_token(lexeme.substr(i + 1, j - i - 1), sigil_type));
					}
					if ((i == 0) && (j < lexeme.size() - 1)) { // "s" b
						tokens.push_back(make_token(lexeme.substr(1, j - i - 1), sigil_type));
						//recursively examine the b section after the closing sigil
						rec_sigil_split(make_token(lexeme.substr(j + 1, lexeme.size() - j + 1), joy_t::undef_t), tokens, open_sigil, close_sigil, sigil_type);
					}
					if ((i > 0) && (j == lexeme.size() - 1)) { // a "s"
						tokens.push_back(make_token(lexeme.substr(0, i - 1), joy_t::undef_t));
						tokens.push_back(make_token(lexeme.substr(i + 1, j - i - 1), sigil_type));
					}
					if ((i > 0) && (j < lexeme.size() - 1)) { // a "s" b
						tokens.push_back(make_token(lexeme.substr(0, i - 1), joy_t::undef_t));
						tokens.push_back(make_token(lexeme.substr(i + 1, j - i - 1), sigil_type));
						//recursively examine the b section after the closing sigil
						rec_sigil_split(make_token(lexeme.substr(j + 1, lexeme.size() - j + 1), joy_t::undef_t), tokens, open_sigil, close_sigil, sigil_type);
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

	joy_stack tokenizer::split_whitespace(joy_stack&& tokens) {
		joy_stack result;
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
		return result; //auto optimizes the move
	}

	joy_stack tokenizer::tokenize_simple_types(joy_stack&& tokens) {
		for (auto& [pattern, type] : tokens) {
			auto match = std::any_cast<std::string>(pattern);
			if (type == joy_t::string_t) { //convert std::string into a joy_stack of char tokens
				joy_stack s;
				for (auto c : match) {
					s.emplace_back(c, joy_t::char_t);
				}
				pattern = s;
			}
			if (jlogical(match)) {
				pattern = (match == "true") ? true : false;
				type = joy_t::bool_t;
				continue;
			}
			if (jchar_space(match)) {
				pattern = ' ';
				type = joy_t::char_t;
				continue;
			}
			if (jchar_escape(match)) {
				switch (match[2]) {
				case 'n':
					pattern = '\n';
					break;
				case 't':
					pattern = '\t';
					break;
				case 'b':
					pattern = '\b';
					break;
				case 'r':
					pattern = '\r';
					break;
				case 'f':
					pattern = '\f';
					break;
				default:
					break;
				}
				type = joy_t::char_t;
				continue;
			}
			if (jchar(match)) {
				pattern = match[1];
				type = joy_t::char_t;
				continue;
			}
			if (jinteger(match)) {
				pattern = stoi(match);
				type = joy_t::int_t;
				continue;
			}
			if (jdouble(match)) {
				pattern = stod(match);
				type = joy_t::double_t;
				continue;
			}
		}
		return tokens;
	}

	void tokenizer::trim(std::string& line) {
		line = std::regex_replace(line, std::regex("^ +"), "");
		line = line.substr(0, line.find_first_of('#'));
	}

}