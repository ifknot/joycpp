#pragma once

constexpr auto ABOUT_INFO = "Joy Interpreter - J.S.Thornton 2019\n" ;

#include <sstream>

#include "trace.h"

#include "joy_stack.h"
#include "io_device.h"
#include "error_messages.h"

#define NSUPRESS

namespace joy {

	class lexer {

	public:

		lexer(joy_stack& stack0, io_device& io, std::string path_to_manual);

		bool is_exit();

		void exit();

	protected:

		/**
		* cascade the lexeme down the regular conversion steps:
		* regular, int, char, double, no conversion
		*/
		virtual bool tokenize(const std::string& lexeme);

		/**
		* test if is a recognized regular Joy command
		*/
		bool is_regular(const std::string& lexeme);

		/**
		* test stack conforms to the requirments of the initializer list
		*/
		bool conforms(const std::initializer_list<joy_t>& argt, const joy_stack& jstack);

		/**
		* perform result cast (if needed) casting to same number type as first item pushed ie 2nd item on stack
		* warn if cast would be a reduction in accuracy
		* pop arguements from stack
		* push result
		*/
		void bin_op_result(token_t& token, joy_stack& stack);

		//void un_op_result

		/**
		* display an error message on the output device
		*/
		void error(error_number_t e, std::string msg = "");

		/**
		* display a warning message on the output device
		*/
		void warn(error_number_t e, std::string msg = "");

		joy_stack& s0;	//stack #0 main stack
		io_device& io;

	private:

		bool exit_{ false };
		std::string about_info{ ABOUT_INFO };

		//lexing cascade:

		/**
		* try and convert lexeme to a regular Joy command 
		* by searching the regular expression c++ dictionary 
		* return true on success
		* return try_int if fail
		*/
		bool try_regular(const std::string& lexeme);

		/**
		* try an and convert lexeme to an int
		* return true on success
		* return try_char if fail
		*/
		bool try_int(const std::string& lexeme);

		/**
		* try an and convert lexeme to a char
		* return true on success
		* return try_double if fail
		*/
		bool try_char(const std::string& lexeme);

		/**
		* try an and convert lexeme to a double
		* return true on success
		* return no_conversion if fail
		*/
		bool try_double(const std::string& lexeme);

		/**
		* display error
		* return false
		*/
		bool no_conversion(const std::string& lexeme);

		//Joy command translations: (where in map lambdas would be too long)

		void load_manual(std::string path);

		void stack_dump(); // .s

		void command_dump(); // ?

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
		{"man", [&]() { if (conforms({joy_t::string_t}, s0)) { man(destring(s0.top().first)); s0.pop(); } }},
		//interpreter environment 
		{TOK_QUIT, [&]() { exit(); io << ". . ."; }},
		{"manual", [&]() { manual(); }},
		{"put", [&]() { if (conforms({joy_t::any_t}, s0)) { io << s0.top().first; s0.pop(); } }},
		{".", [&]() { if (conforms({joy_t::any_t}, s0)) { io << s0.top().first; } }},
		//stack commands
		{".s", [&]() { stack_dump(); }},
		{"newstack", [&]() { s0.newstack(); }},
		{"stack", [&]() { if (conforms({joy_t::any_t}, s0)) { s0.stack(); } }},
		{"unstack", [&]() { if (conforms({joy_t::quote_t}, s0)) { s0.unstack(); } }},
		{"dup", [&]() { if (conforms({joy_t::any_t}, s0)) { s0.dup(); } }},
		{"dupd", [&]() { if (conforms({joy_t::any_t, joy_t::any_t}, s0)) { s0.dupd(); } }},
		{"pop", [&]() { if (conforms({joy_t::any_t}, s0)) { s0.pop(); } }},
		{"popd", [&]() { if (conforms({joy_t::any_t, joy_t::any_t}, s0)) { s0.popd(); } }},
		{"pop2", [&]() { if (conforms({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s0)) { s0.pop2(); } }},
		{"unit", [&]() { if (conforms({joy_t::any_t}, s0)) { s0.unit(); } }},
		{"swap", [&]() { if (conforms({joy_t::any_t, joy_t::any_t}, s0)) { s0.swap(); } }},
		{"swapd", [&]() { if (conforms({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s0)) { s0.swapd(); } }},
		{"rotate", [&]() { if (conforms({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s0)) { s0.rotate(); } }},
		{"rollup", [&]() { if (conforms({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s0)) { s0.rollup(); } }},
		{"rolldown", [&]() { if (conforms({joy_t::any_t, joy_t::any_t, joy_t::any_t}, s0)) { s0.rolldown(); } }},
		//boolean
		{"true", [&]() { s0.push(make_token("true", joy_t::bool_t)); }},
		{"false", [&]() { s0.push(make_token("false", joy_t::bool_t)); }},
		//char
		{"ord", [&]() { 
			if (conforms({joy_t::char_t}, s0)) {
				s0.push(make_token(std::to_string(static_cast<int>(s0.top().first[1])), joy_t::int_t));
				s0.popd();
			} 
		}},
		{"chr", [&]() { 
			if (conforms({joy_t::int_t}, s0)) {
				auto c = static_cast<char>(stoi(s0.top().first)); 
				s0.push(make_token("'" + std::string{c}, joy_t::char_t)); 
				s0.popd(); 
			} 
		}},
		{"char", [&]() { 
			if (conforms({joy_t::any_t}, s0)) {
				tokenize((s0.top().second == joy_t::char_t) ? "true" : "false"); 
			} 
		}},
		//math
		{"+", [&]() { if (conforms({joy_t::number_t, joy_t::number_t}, s0)) { } }},
		/*
		{"-", [&]() { if (args(2)) { } }},
		{"*", [&]() { if (args(2)) { } }},
		{"/", [&]() { if (args(2)) { } }},
		{"rem", [&]() { if (args(1)) { } }},
		{"abs", [&]() { if (args(1)) { } }},
		{"sign", [&]() { if (args(1)) { } }},
		*/
		//identity function, does nothing.
		{"id", [&]() {}}
		};

		/**
		* Joy Manual loaded from file at construction
		*/
		std::map<std::string, std::string> joy_manual;

	};

}

