#include "joy_stack.h"

namespace joy {

#ifndef NDEBUG
	void joy_stack::dump() {
		std::cerr << "_\n";
		for (size_t i{ 0 }; i < size(); ++i) {
			const auto& [pattern, joy_type] = sat(i);
			std::cerr << pattern << " " << to_string(joy_type) << "\n";
		}
	}
#endif

	base_stack_t::const_reference joy_stack::sat(size_t i) const {
		assert(i < size());
		return at(size() - 1 - i);
	}

	base_stack_t::reference joy_stack::sat(size_t i) {
		assert(i < size());
		return at(size() - 1 - i);
	}

	void joy_stack::push(const value_type& val) {
		push_back(val);
	}

	void joy_stack::push(value_type&& val) {
		push_back(val);
	}

	void joy_stack::newstack() {
		clear();
	}

	void joy_stack::stack() {
		pattern_t lexeme;
		for (size_t i{ 0 }; i < size(); ++i) {
			const auto& [pattern, joy_type] = at(i);
			lexeme += pattern + SPC;
		}
		push(make_token(add_sigils(lexeme, QUOTE_OPEN, QUOTE_CLOSE), joy_t::quote_t));
	}

	void joy_stack::semi_stack() {
		pattern_t lexeme;
		for (size_t i{ 0 }; i < size(); ++i) {
			const auto& [pattern, joy_type] = at(i);
			lexeme += pattern + SPC;
		}
		push(make_token(lexeme, joy_t::quote_t));
	}

	void joy_stack::unstack() {
		assert(top().second == joy_t::quote_t);
		pattern_t p;
		std::stringstream ss{ top().first };
		pop();
		newstack();
		while (ss >> p) {
			//push(make_token(p, joy_type(p)));
		}
	}

	void joy_stack::dup() {
		if (size()) {
			push(top());
		}
	}

	void joy_stack::dupd() {
		if (size()) {
			dup();
			sat(1) = sat(2);
		}
	}

	void joy_stack::pop() {
		if (size()) {
			pop_back();
		}
	}

	void joy_stack::popd() {
		if (size() > 1) {
			sat(1) = sat(0);
			pop();
		}
	}

	void joy_stack::pop2() {
		if (size() > 1) {
			pop();
			pop();
		}
	}

	void joy_stack::unit() {
		top() = make_token(add_sigils(top().first, QUOTE_OPEN, QUOTE_CLOSE), joy_t::quote_t);
	}

	void joy_stack::swap() {
		if (size() > 1) {
			std::swap(sat(0), sat(1));
		}
	}

	void joy_stack::swapd() {
		if (size() > 1) {
			std::swap(sat(1), sat(2));
		}
	}

	void joy_stack::rotate() {
		if (size() > 2) {
			std::swap(sat(0), sat(2));
		}
	}

	void joy_stack::rollup() {
		swap();
		swapd();
	}

	void joy_stack::rolldown() {
		rotate();
		swapd();
	}

	base_stack_t::reference joy_stack::top() {
		if (size()) {
			return back();
		}
		else {
			throw std::runtime_error(NULL_STACK);
		}
	}

	base_stack_t::const_reference joy_stack::top() const {
		if (size()) {
			return back();
		}
		else {
			throw std::runtime_error(NULL_STACK);
		}
	}


}