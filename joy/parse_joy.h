#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <map>

#include "parse_context_free.h"

// [ [ 1 2 3 ] [ 4 5 6 ] ] [ [ cube ] map ] map .

namespace joy {

	class parse_joy : public parse_context_free {

		enum class joy_state_t{ parse, candidate, define };

	public:

		using parse_context_free::parse_context_free;

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

		joy_state_t joy_state{ joy_state_t::parse };

		std::string command;
		std::string definition;
		std::string module;

		joy_stack tokenize_joy_commands(joy_stack&& tokens);

		bool validate_tokens(joy_stack&& tokens);

		void include(std::string&& path);

		//TODO: joy_lambda_map MODULE PUBLIC PRIVATE END LIBRA DEFINE
		//DEFINE can over-ride
		//see userlib.joy DEFINE verbose == true. (* Example of over-riding  inilib.joy *)

		dictionary_t joy_lambda_map{
			{"include", [&](joy_stack& S) { 
				if (S.has("include", {joy_t::string_t})) {
					include(joy_stack::to_string(S.top()));
				}
			}},
		};

		std::map<std::string, std::string> public_joy_joy_map {
			{"cube", "dup dup * *"},
			{"square", "dup *"}
		};

		std::map<std::string, std::string> private_joy_joy_map{
			{"Q", "dup dup * *"},
			{"S", "dup *"}
		};


	};

}