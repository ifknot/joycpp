#pragma once

#include "parse_context_free.h"

namespace joy {

	class parse_joy : public parse_context_free {

		enum class defn_state_t{ joy, candidate, define };

	public:

		using parse_context_free::parse_context_free;

		/**
		* TODO: remove after testing
		*/
		inline bool root_parse(joy_stack& tokens) {
			return(parse_context_free::parse(tokens, root_stack));
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
		* nested parse a quoted program P against a stack S
		*/
		virtual bool parse(token_t& token, joy_stack& S) override;

	private:

		defn_state_t defn_state;


	};

}