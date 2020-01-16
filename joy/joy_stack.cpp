#include "joy_stack.h"

namespace joy {

	void joy_stack::newstack() {
		clear();
	}

	void joy_stack::stack() {
		joy_stack S = *this;
		std::reverse(S.begin(), S.end());
		push(make_token(S, joy_t::quote_t));
	}

	void joy_stack::unstack() {
		auto S = std::any_cast<joy_stack>(top().first);
		std::reverse(S.begin(), S.end());
		*this = S;
	}

	void joy_stack::dup() {
		assert(size());
		push(top());
	}

	void joy_stack::dupd() {
		assert(size() > 1);
		dup();
		sat(1) = sat(2);
	}

	void joy_stack::pop() {
		assert(size());
		pop_back();
	}

	void joy_stack::popd() {
		assert(size() > 1);
		sat(1) = sat(0);
		pop();
	}

	void joy_stack::pop2() {
		assert(size() > 1);
		pop();
		pop();
	}

	void joy_stack::unit() {
		joy_stack S;
		S.push(top());
		top() = make_token(S, joy_t::quote_t);
	}

	void joy_stack::swap() {
		assert(size() > 1);
		std::swap(sat(0), sat(1));
	}

	void joy_stack::swapd() {
		assert(size() > 1);
		std::swap(sat(1), sat(2));
	}

	void joy_stack::rotate() {
		assert(size() > 2);
		std::swap(sat(0), sat(2));
	}

	void joy_stack::rollup() {
		swap();
		swapd();
	}

	void joy_stack::rolldown() {
		rotate();
		swapd();
	}

	joy_stack::const_reference joy_stack::sat(size_t i) const {
		assert(i < size());
		return at(size() - 1 - i);
	}

	joy_stack::reference joy_stack::sat(size_t i) {
		assert(i < size());
		return at(size() - 1 - i);
	}

	joy_stack::reference joy_stack::top() {
		assert(size());
		return back();
	}

	joy_stack::const_reference joy_stack::top() const {
		assert(size());
		return back();
	}

	void joy_stack::push(const value_type& val) {
		push_back(val);
	}

	void joy_stack::push(value_type&& val) {
		push_back(val);
	}

}