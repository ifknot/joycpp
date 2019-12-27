#pragma once

#include <sstream>

#include "joy_stack.h"
#include "io_device.h"
#include "error_messages.h"

namespace joy {

	class lexer {

	public:

		lexer(joy_stack& js, io_device& io, std::string path_to_manual);

		bool operator()(std::string&& line);

		bool is_exit();

		void exit();

	protected:

		virtual bool tokenize(const std::string& lexeme);

		/**
		* search the regular expression c++ dictionary for a recognised Joy command
		* cascade down to test for simple types
		* finally display unrecognised input if no conversion
		*/
		bool try_regular(const std::string& lexeme);

		bool try_string(const std::string& lexeme);

		bool try_int(const std::string& lexeme);

		bool try_char(const std::string& lexeme);

		bool try_double(const std::string& lexeme);

		bool no_conversion(const std::string& lexeme);

		virtual void err(error_number_t e, std::string msg = "");

		//belongs in context free (later)
		virtual void joy_include();

		joy_stack& js;
		io_device& io;

	private:

		void load_manual(std::string path);

		bool args(size_t n);

		bool expects(size_t argc, joy_t argt);

		//Joy command translations

		void stack_dump();

		void command_dump();

		void joy_assert(pattern_t expected, pattern_t found);

		void man(std::string match);

		void manual();

		// vars

		bool exit_{ false };
		std::string about_info{ "Joy Interpreter - J.S.Thornton 2019\n" };

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
		{"man", [&]() { if (expects(1, joy_t::string_t)) { man(destring(js.top().first)); js.pop(); } }},
		{"assert", [&]() { if (args(2)) { joy_assert(js.top().first, js.sat(1).first); } }},
		//interpreter environment 
		{TOK_QUIT, [&]() { exit(); io << ". . ."; }},
		{"manual", [&]() { manual(); }},
		//belongs in context free (later)
		{"include", [&]() { if (expects(1, joy_t::string_t)) { joy_include(); } }},
		{"put", [&]() { if (args(1)) { io << js.top().first; js.pop(); } }},
		//boolean simple types
		{TOK_TRUE, [&]() { js.push(make_token("true", joy_t::bool_t)); }},
		{TOK_FALSE, [&]() { js.push(make_token("false", joy_t::bool_t)); }},
		//stack commands
		{".s", [&]() { stack_dump(); }},
		{"newstack", [&]() { js.newstack(); }},
		{"stack", [&]() { if (args(1)) { js.stack(); } }},
		{"unstack", [&]() { if (expects(1, joy_t::quote_t)) { js.unstack(); } }},
		{"dup", [&]() { if (args(1)) { js.dup(); } }},
		{"dupd", [&]() { if (args(2)) { js.dupd(); } }},
		{"pop", [&]() { if (args(1)) { js.pop(); } }},
		{"popd", [&]() { if (args(2)) { js.popd(); } }},
		{"pop2", [&]() { if (args(3)) { js.pop2(); } }},
		{"unit", [&]() { if (args(1)) { js.unit(); } }},
		{"swap", [&]() { if (args(2)) { js.swap(); } }},
		{"swapd", [&]() { if (args(3)) { js.swapd(); } }},
		{"rotate", [&]() { if (args(3)) { js.rotate(); } }},
		{"rollup", [&]() { if (args(3)) { js.rollup(); } }},
		{"rolldown", [&]() { if (args(3)) { js.rolldown(); } }},
		{"id", [&]() { }}
		};

		/**
		* Joy Manual loaded from file at construction
		*/
		std::map<std::string, std::string> joy_manual;

	};

}

