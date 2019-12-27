#pragma once

#include <sstream>

#include "joy_stack.h"
#include "io_device.h"
#include "error_messages.h"

namespace joy {

	class lexer {

	public:

		lexer(joy_stack& js, io_device& io);

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

		bool try_bool(std::string lexeme);

		bool try_int(std::string lexeme);

		bool try_char(std::string lexeme);

		bool try_double(std::string lexeme);

		bool no_conversion(std::string lexeme);

		virtual void err(error_number_t e);

		joy_stack& js;
		io_device& io;

	private:

		//Joy command translations

		void stack_dump();

		void command_dump();

		bool exit_{ false };

		/**
		* translate Joy regular grammar commands into their c++ lambda equivalents
		* only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		cpp_dictionary_t regular_translation {
		{TOK_QUIT, [&]() { exit(); io << ". . ."; }},
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

	};

}

