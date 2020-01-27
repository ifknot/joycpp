#pragma once

#include <cassert>

#include "error.h"
#include "joy_stack.h"
#include "tokenizer.h"
#include "joy_overloads.h"
#include "joy_operators.h"

namespace joy {

	/**
	* Chomsky type 3 regular grammar lexer
	* i.e. can be decided by a finite state automaton
	* Uses:
	* 1. root_stack
	* 2. map of string Joy operator to lambda function mapping for regular grammar C++ implemented Joy operators
	*/
	class parse_regular : public tokenizer {

	public:

		parse_regular(joy_stack& stack, io_device& io, std::string path_to_manual);

		bool is_quit();

		void quit();

	protected:

		typedef std::function<void(joy_stack & S)> function_t;
		typedef std::map<std::string, function_t> dictionary_t;

		/**
		* TODO: test performance if reserve large stack space at construction
		* root stack
		*/
		joy_stack& root_stack;	

		/**
		* root type
		*/
		joy_t root_type{ joy_t::quote_t };

		using tokenizer::tokenize;

		/**
		* cascade tokens to tokenizer type id the simple type
		* then regular grammar type id
		*/
		virtual joy_stack tokenize(joy_stack& tokens) override;

		/**
		* display errors for any undefined tokens
		*/
		virtual void no_conversion(joy_stack& tokens);

		/**
		* operator matching function and execute if match return true otherwise return false
		*/
		virtual bool call(token_t& token, joy_stack& S);

	private: 
		
		/**
		* tokenize regular expression Joy command types
		*/
		joy_stack tokenize_regular_types(joy_stack&& tokens);

		/**
		* internal quit flag
		*/
		bool quit_{ false };

		/**
		* load a Joy manual in Thun's original manual format but with the addition of:
		* # a line must start with # to be a comment in a manual file
		* "JOY - compiled at 16: 57: 51 on Mar 17 2003(BDW) Copyright 2001 by Manfred von Thun"
		* Manual entry format:
		* command_name : command algebra
		* command summary
		* e.g.
		* rolldown : X Y Z -> Y Z X
		* Moves Y and Z down, moves X up
		*/
		void load_manual(std::string& path_to_manual);

		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		dictionary_t regular_lambda_map { 
		//non-standard
		{"?", [&](joy_stack& S) { if (S.has("?", {joy_t::quote_t})) { helpdetail(std::any_cast<joy_stack&>(S.top().first), joy_manual, io); } }},
		//stack commands
		{".s", [&](joy_stack& S) { print_stack(S, io); }},
		{"put", [&](joy_stack& S) { if (S.has(".", {joy_t::any_t})) { print_top(S, io); S.pop(); } }},
		{"newstack", [&](joy_stack& S) { S.newstack(); }},
		{"stack", [&](joy_stack& S) { S.stack(); }},
		{"unstack", [&](joy_stack& S) { S.unstack(); }},
		{"dup", [&](joy_stack& S) { S.dup(); }},
		{"dupd", [&](joy_stack& S) { S.dupd(); }},
		{"pop", [&](joy_stack& S) { S.pop(); }},
		{"popd", [&](joy_stack& S) { S.popd(); }},
		{"pop2", [&](joy_stack& S) { S.pop2(); }},
		{"unit", [&](joy_stack& S) { S.unit(); }},
		{"swap", [&](joy_stack& S) { S.swap(); }},
		{"swapd", [&](joy_stack& S) { S.swapd(); }},
		{"rotate", [&](joy_stack& S) { S.rotate(); }},
		{"rollup", [&](joy_stack& S) { S.rollup(); }},
		{"rolldown", [&](joy_stack& S) { S.rolldown(); }},
		//char
		{"ord", [&](joy_stack& S) { if (S.has("ord", {joy_t::char_t})) { S.push(ord(S.top())); } }},
		{"chr", [&](joy_stack& S) { if (S.has("chr", {joy_t::int_t})) { S.push(chr(S.top())); } }},
		{"char", [&](joy_stack& S) { if (S.has("char", {joy_t::any_t})) { S.push(is_char(S.top())); } }},
		//math
		{"+", [&](joy_stack& S) {
			if (S.has("+", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.sat(1) = (S.sat(1) + S.top());
				S.pop();
			}
		}},
		{"-", [&](joy_stack& S) {
			if (S.has("-", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.sat(1) = (S.sat(1) - S.top());
				S.pop();
			}
		}},
		{"*", [&](joy_stack& S) {
			if (S.has("*", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.sat(1) = (S.sat(1) * S.top());
				S.pop();
			}
		}},
		{"/", [&](joy_stack& S) {
			if (S.has("/", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.sat(1) = (S.sat(1) / S.top());
				S.pop();
			}
		}},
		{"==", [&](joy_stack& S) { 
			if (S.has("==", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.push(S.sat(1) == S.top());
			}
		}},
		//aggregates
		// size  ==  [ pop 1 ]  map  sum
		{"size", [&](joy_stack& S) { size(S); }},
		//environment
		{"manual", [&](joy_stack& S) { manual(joy_manual, io); }},
		{"quit", [&](joy_stack& S) { quit(); io << ". . ."; }}
		};

		/**
		* Joy Manual loaded from file at construction
		*/
		std::map<std::string, std::string> joy_manual;

	};

}
