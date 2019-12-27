#pragma once

#include <sstream>

#include "joy_stack.h"
#include "io_device.h"
#include "error_messages.h"

namespace joy {

	class lexer {

	public:

		lexer(joy_stack& js, io_device& io, std::string path_to_manual);

		virtual bool operator()(std::string&& line);

		bool is_exit();

		void exit();

	protected:

		/**
		* search the regular expression c++ dictionary for a recognised Joy command
		* cascade down to test for simple types
		* finally display unrecognised input if no conversion
		*/
		bool try_regular(std::string lexeme);

		bool try_string(std::string lexeme);

		bool try_int(std::string lexeme);

		bool try_char(std::string lexeme);

		bool try_double(std::string lexeme);

		bool no_conversion(std::string lexeme);

		virtual void err(error_number_t e, std::string msg = "");

		virtual void man(std::string command);

		joy_stack& js;
		io_device& io;

	private:

		bool expects(size_t argc, joy_t argt);

		//Joy command translations

		void stack_dump();

		void command_dump();

		void lex_file();

		void man();

		// vars

		bool exit_{ false };

		/**
		* Joy03 (language specs asper Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents
		* only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		cpp_dictionary_t regular_translation {
		//non standard commands
		{"man", [&]() {man(); }},
		//interpreter environment 
		{TOK_QUIT, [&]() { exit(); io << ". . ."; }},
		{"include", [&]() { lex_file(); }},
		//boolean simple types
		{TOK_TRUE, [&]() { js.push(make_token("true", joy_t::bool_t)); }},
		{TOK_FALSE, [&]() { js.push(make_token("false", joy_t::bool_t)); }},
		//stack commands
		{".s", [&]() { stack_dump(); }},
		{"newstack", [&]() { js.newstack(); }},
		{"stack", [&]() { js.stack(); }},
		{"unstack", [&]() { js.unstack(); }},
		{"dup", [&]() { js.dup(); }},
		{"dupd", [&]() { js.dupd(); }},
		{"pop", [&]() { js.pop(); }},
		{"popd", [&]() { js.popd(); }},
		{"pop2", [&]() { js.pop2(); }},
		{"unit", [&]() { js.unit(); }},
		{"swap", [&]() { js.swap(); }},
		{"swapd", [&]() { js.swapd(); }},
		{"rotate", [&]() { js.rotate(); }},
		{"rollup", [&]() { js.rollup(); }},
		{"rolldown", [&]() { js.rolldown(); }},

		{"?", [&]() { command_dump(); }}
		};

		/**
		* Joy Manual loaded from file at construction
		*/
		std::map<std::string, std::string> manual;

	};

}

