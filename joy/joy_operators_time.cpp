#include "joy_operators_time.h"

#include <ctime>
#include <chrono>

namespace joy {

	void clock(joy_stack& S) {
		auto cpu_usage = std::clock() / (CLOCKS_PER_SEC / 10);
		S.push(make_token(static_cast<int>(cpu_usage), joy_t::int_t));
	}

	void time(joy_stack& S) {
		// use <chrono> as an alternative approach 
		int seconds_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(
				std::chrono::system_clock::now().time_since_epoch()
			).count();
		S.push(make_token(seconds_since_epoch, joy_t::int_t));
	}

}
