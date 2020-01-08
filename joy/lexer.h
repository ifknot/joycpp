/**
* Chomsky type 3 regular grammar lexer 
* Uses a map of string operator to lamda function mapping for Joy operators
* Cascades down to then test for simple types terminating in unrecognised input error
*/
#pragma once

#include "tokenizer.h"
#include "joy_stack.h"
#include "error_messages.h"


namespace joy {

	class lexer : public tokenizer {

	public:

		lexer(joy_stack& stack, io_device& io, std::string path_to_manual);

		void lex(std::string line);

		bool is_quit();

		void quit();

	protected:

		joy_stack& s;	

		bool try_regular(const token_t& token);

		void error(error_number_t e, std::string msg = "");

		void print_stack(const joy_stack& stack);

		/**
		* test stack has to the requirments of the initializer list
		*/
		bool has(const std::initializer_list<joy_t>& argt, const joy_stack& stack);

	private:

		bool no_conversion(const token_t& token);

		bool quit_{ false };

		dictionary_t regular_translation { 
		//stack commands
		{".s", [&]() { print_stack(s); }},
		//{"newstack", [&]() { s.newstack(); }},
		//{"stack", [&]() { if (has({joy_t::any_t}, s)) { s.stack(); } }},
		//{"unstack", [&]() { if (has({joy_t::quote_t}, s)) { s.unstack(); } }},
		{"dup", [&]() { if (has({joy_t::any_t}, s)) { s.dup(); } }},
		{"dupd", [&]() { if (has({joy_t::any_t, joy_t::any_t}, s)) { s.dupd(); } }},
		{"pop", [&]() { if (has({joy_t::any_t}, s)) { s.pop(); } }},
		{"popd", [&]() { if (has({joy_t::any_t, joy_t::any_t}, s)) { s.popd(); } }},
		{"pop2", [&]() { if (has({joy_t::any_t, joy_t::any_t}, s)) { s.pop2(); } }},
		{"unit", [&]() { if (has({joy_t::any_t}, s)) { s.unit(); } }},
		{"swap", [&]() { if (has({joy_t::any_t, joy_t::any_t}, s)) { s.swap(); } }},
		{"swapd", [&]() { if (has({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s)) { s.swapd(); } }},
		{"rotate", [&]() { if (has({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s)) { s.rotate(); } }},
		{"rollup", [&]() { if (has({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s)) { s.rollup(); } }},
		{"rolldown", [&]() { if (has({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s)) { s.rolldown(); } }},
		//boolean
		{"true", [&]() { s.push(make_token(true, joy_t::bool_t)); }},
		{"false", [&]() { s.push(make_token(false, joy_t::bool_t)); }},
		//environment
		{"quit", [&]() { quit(); io << ". . ."; }}
		};

	};

}