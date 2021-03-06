#include "tokenizer.h"

#include <iostream>

namespace joy {

	tokenizer::tokenizer(io_device& io) : io(io) {}

	joy_stack tokenizer::tokenize(std::string& line) {
		echo(line_number++, line);
		joy_stack tokens;
		trim_leading_whitespace(line);
		trim_trailing_comment(line);
		if (line.empty()) { //ignore any empty line 
			return tokens;
		}
		tokens.push_back(make_token(line, joy_t::undef_t)); //entire line as a single undefined token
		return tokenize(tokens);
	}

	joy_stack tokenizer::tokenize(std::string&& line) {
		echo(line_number++, line);
		joy_stack tokens;
		trim_leading_whitespace(line);
		trim_trailing_comment(line);
		if (line.empty()) { //ignore any empty line 
			return tokens;
		}
		tokens.push_back(make_token(line, joy_t::undef_t)); //entire line as a single undefined token
		return tokenize(tokens);
	}

	joy_stack tokenizer::tokenize(joy_stack& tokens) {
		return
			tokenize_simple_types(
				tokenize_period (
				strip_comments(
					split_whitespace(
						tokenize_strings(
							tokenize_reserved(std::move(tokens))
						)
					)
				)
				)
			);
	}

	void tokenizer::echo(size_t line_number, const std::string& line) {
		io.colour(YELLOW);
		switch (echo_state) {
		case echo_state_t::linenumber:
			io << std::to_string(line_number) + "\t" + line;
			break;
		case echo_state_t::tab:
			io << "\t" + line;
			break;
		case echo_state_t::echo:
			io << line;
			break;
		case echo_state_t::none:
		default:
			break;
		}
	}

	joy_stack tokenizer::tokenize_strings(joy_stack&& tokens) {
		joy_stack result;
		for (const auto& token : tokens) {  //examine all the tokens
			rec_sigil_split(token, result, "\"", "\"", joy_t::string_t); //recursively find and split out open-close quotes into tokens
		}
		return result;
	}

	joy_stack tokenizer::tokenize_reserved(joy_stack&& tokens) {
		joy_stack result;
		for (const auto r : reserved) {
			for (const auto& token : tokens) {
				rec_char_split(token, result, r, joy_t::undef_t);
			}
			tokens = result;
			result.clear();
		}
		return tokens;
	}

	joy_stack tokenizer::tokenize_period(joy_stack&& tokens) {
		joy_stack result;
		for (const auto& token : tokens) {
			auto lexeme = joy_cast<std::string>(token);
			if (lexeme.size() > 1 && lexeme[0] != '\'' && lexeme[lexeme.size() - 1] == '.') { 
				//split into 2 tokens - lexeme and its trailing '.'
				result.push_back(make_token(lexeme.substr(0, lexeme.size() - 1), joy_t::undef_t));
				result.push_back(make_token(lexeme.substr(lexeme.size() - 1, 1), joy_t::undef_t));
			}
			else {
				result.push_back(token);
			}
		}
		return result;;
	}

	void tokenizer::rec_char_split(token_t token, joy_stack& tokens, char ch, joy_t char_type) {
		if (jundef(token)) {
			auto lexeme = joy_cast<std::string>(token);
			auto i = lexeme.find(ch);
			if (i < lexeme.size()) { 
				tokens.push_back(make_token(lexeme.substr(0, i), joy_t::undef_t));
				tokens.push_back(make_token(lexeme.substr(i, 1), char_type));
				if (i < lexeme.size() - 1) {
					rec_char_split(make_token(lexeme.substr(i + 1, lexeme.size() - i), joy_t::undef_t), tokens, ch, char_type);
				}
			}
			else {
				tokens.push_back(token);
			}
		}
		else { 
			tokens.push_back(token);
		}
	}

	void tokenizer::rec_sigil_split(token_t token, joy_stack& tokens, std::string open_sigil, std::string close_sigil, joy_t sigil_type) {
		auto k1 = open_sigil.size();
		auto k2 = close_sigil.size();
		if (jundef(token)) {
			auto lexeme = joy_cast<std::string>(token);
			auto i = lexeme.find(open_sigil);
			if (i < lexeme.size()) { //found an opening sigil
				auto j = lexeme.find(close_sigil, i + k1);
				if (j < lexeme.size()) { //found a closing sigil
					//split the token up into first sigil enclosed section (s) and any undefs (a) before (s) and recurse on any undefs (b) after (s)
					if ((i == 0) && (j == lexeme.size() - 1)) { // "s"
						tokens.push_back(make_token(lexeme.substr(i + k1, j - i - k2), sigil_type));
					}
					if ((i == 0) && (j < lexeme.size() - k2)) { // "s" b
						tokens.push_back(make_token(lexeme.substr(1, j - i - k2), sigil_type));
						//recursively examine the b section after the closing sigil
						rec_sigil_split(make_token(lexeme.substr(j + k1, lexeme.size() - j + k2), joy_t::undef_t), tokens, open_sigil, close_sigil, sigil_type);
					}
					if ((i > 0) && (j == lexeme.size() - 1)) { // a "s"
						tokens.push_back(make_token(lexeme.substr(0, i - 1), joy_t::undef_t));
						tokens.push_back(make_token(lexeme.substr(i + k1, j - i - k2), sigil_type));
					}
					if ((i > 0) && (j < lexeme.size() - 1)) { // a "s" b
						tokens.push_back(make_token(lexeme.substr(0, i - k1), joy_t::undef_t));
						tokens.push_back(make_token(lexeme.substr(i + k1, j - i - k2), sigil_type));
						//recursively examine the b section after the closing sigil
						rec_sigil_split(make_token(lexeme.substr(j + k1, lexeme.size() - j + k2), joy_t::undef_t), tokens, open_sigil, close_sigil, sigil_type);
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

	joy_stack tokenizer::strip_comments(joy_stack&& tokens) {
		joy_stack result;
		for (const auto& token : tokens) {  //examine all the tokens
			if (comment_block) {
				if (token == "*)") {
					comment_block = false;
					continue;
				}
			}
			if (!comment_block) {
				if (token == "(*") {
					comment_block = true;
				}
				else {
					result.push_back(token);
				}
			}
		}
		return result;
	}

	joy_stack tokenizer::split_whitespace(joy_stack&& tokens) {
		joy_stack result;
		for (const auto& t : tokens) { //examine all the tokens
			if (t.second == joy_t::undef_t) { //check only undef tokens 
				std::stringstream ss(joy_cast<std::string>(t));
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
		for (auto& token : tokens) {
			auto& [pattern, type] = token;
			auto match = joy_cast<std::string>(token);
			if (type == joy_t::string_t) { //convert std::string into a joy_stack of char tokens
				joy_stack s;
				for (auto c : match) {
					s.emplace_back(c, joy_t::char_t);
				}
				pattern = s;
			}
			else {
				convert_simple(token);
			}
		}
		return std::move(tokens);
	}

}