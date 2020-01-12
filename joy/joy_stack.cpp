#include "joy_stack.h"

namespace joy {

	token_list_t::const_reference joy_stack::sat(size_t i) const {
		assert(i < size());
		return at(size() - 1 - i);
	}

	token_list_t::reference joy_stack::sat(size_t i) {
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
		joy_stack S = *this;
		push(make_token(S, joy_t::quote_t));
	}

	void joy_stack::unstack() {
		auto S = std::any_cast<joy_stack>(top().first);
		*this = S;
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

	joy_stack::reference joy_stack::top() {
		if (size()) {
			return back();
		}
		else {
			throw std::runtime_error("");
		}
	}

	joy_stack::const_reference joy_stack::top() const {
		if (size()) {
			return back();
		}
		else {
			throw std::runtime_error("");
		}
	}


}