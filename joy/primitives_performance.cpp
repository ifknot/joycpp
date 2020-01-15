#include "primitives_performance.h"

namespace joy {

	void reverse(joy_stack& stack) {
		auto& A = std::any_cast<joy_stack&>(stack.top().first);
		std::reverse(A.begin(), A.end());
	}

}