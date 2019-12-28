#pragma once

constexpr auto ABOUT_INFO = "Joy Interpreter - J.S.Thornton 2019\n" ;

#include <sstream>

#include "trace.h"

#include "joy_stack.h"
#include "io_device.h"
#include "error_messages.h"

namespace joy {

	class lexer {

	public:

		lexer(joy_stack& stack0, io_device& io, std::string path_to_manual);

		bool is_exit();

		void exit();

	protected:

		virtual bool tokenize(const std::string& lexeme);

		virtual void err(error_number_t e, std::string msg = "");

		bool args(size_t n);

		bool expects(size_t argc, joy_t argt);

		joy_stack& s0;	//stack #0 main stack
		io_device& io;

	private:

		void load_manual(std::string path);

		/**
		* search the regular expression c++ dictionary for a recognised Joy command
		* cascade down to test for simple types
		* finally display unrecognised input if no conversion
		*/
		bool try_regular(const std::string& lexeme);

		bool try_int(const std::string& lexeme);

		bool try_char(const std::string& lexeme);

		bool try_double(const std::string& lexeme);

		bool no_conversion(const std::string& lexeme);

		bool exit_{ false };
		std::string about_info{ ABOUT_INFO };

		//Joy command translations

		void stack_dump();

		void command_dump();

		void man(std::string match);

		void manual();


		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents
		* only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		cpp_dictionary_t regular_translation {
		//non standard commands
		{"?", [&]() { command_dump(); }},
		{"about", [&]() { io.colour(YELLOW);  io << about_info; }},
		{"man", [&]() { if (expects(1, joy_t::string_t)) { man(destring(s0.top().first)); s0.pop(); } }},
		//interpreter environment 
		{TOK_QUIT, [&]() { exit(); io << ". . ."; }},
		{"manual", [&]() { manual(); }},
		{"put", [&]() { if (args(1)) { io << s0.top().first; s0.pop(); } }},
		//boolean simple types
		{TOK_TRUE, [&]() { s0.push(make_token("true", joy_t::bool_t)); }},
		{TOK_FALSE, [&]() { s0.push(make_token("false", joy_t::bool_t)); }},
		//stack commands
		{".s", [&]() { stack_dump(); }},
		{"newstack", [&]() { s0.newstack(); }},
		{"stack", [&]() { if (args(1)) { s0.stack(); } }},
		{"unstack", [&]() { if (expects(1, joy_t::quote_t)) { s0.unstack(); } }},
		{"dup", [&]() { if (args(1)) { s0.dup(); } }},
		{"dupd", [&]() { if (args(2)) { s0.dupd(); } }},
		{"pop", [&]() { if (args(1)) { s0.pop(); } }},
		{"popd", [&]() { if (args(2)) { s0.popd(); } }},
		{"pop2", [&]() { if (args(3)) { s0.pop2(); } }},
		{"unit", [&]() { if (args(1)) { s0.unit(); } }},
		{"swap", [&]() { if (args(2)) { s0.swap(); } }},
		{"swapd", [&]() { if (args(3)) { s0.swapd(); } }},
		{"rotate", [&]() { if (args(3)) { s0.rotate(); } }},
		{"rollup", [&]() { if (args(3)) { s0.rollup(); } }},
		{"rolldown", [&]() { if (args(3)) { s0.rolldown(); } }},
		{"id", [&]() { }}
		};

		/**
		* Joy Manual loaded from file at construction
		*/
		std::map<std::string, std::string> joy_manual;

	};

}

