#pragma once

#include <string>
#include <sstream>
#include <map>

#include "parse_context_free.h"

// [ [ 1 2 3 ] [ 4 5 6 ] ] [ [ cube ] map ] map .

namespace joy {

	class parse_joy : public parse_context_free {

		enum class defn_state_t{ parse, candidate, define };

	public:

		parse_joy(joy_stack& stack, io_device& io, std::string path_to_manual);

		/**
		* TODO: remove after testing
		*/
		inline bool root_parse(joy_stack& tokens) {
			return parse_context_free::parse(tokens, root_stack);
		}

		using tokenizer::tokenize;

		using parse_context_free::state;

		/**
		* cascade tokens to tokenizer type id the simple type
		* then regular grammar type id
		* then context free grammar type id
		* then eol and eof type id
		*/
		virtual joy_stack tokenize(joy_stack&& tokens) override;

	protected:

		/**
		* executes Joy operators defined as Joy operators
		* operator matching function and execute if match return true otherwise return false
		*/
		virtual bool call(token_t& token, joy_stack& S) override;

		/**
		* nested parse a quoted program P against a stack S
		*/
		virtual bool parse(token_t& token, joy_stack& S) override;

	private:

		defn_state_t defn_state;

		joy_stack tokenize_joy_commands(joy_stack&& tokens);

		std::map<std::string, std::string> joy_joy_map {
			{"cube", "dup dup * *"},
			{"square", "dup *"}
		};


	};

}