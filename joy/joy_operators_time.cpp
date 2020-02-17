#include "joy_operators_time.h"

#include <ctime>
#include <errno.h> 
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

	void localtime(joy_stack& S) {
		time_t raw_time = std::any_cast<int>(S.top().first);
		S.pop();
		struct tm local_time;
		std::time(&raw_time);
		localtime_s(&local_time, &raw_time);
		joy_stack M;
		M.push(make_token(local_time.tm_year, joy_t::int_t));
		M.push(make_token(local_time.tm_mon, joy_t::int_t));
		M.push(make_token(local_time.tm_hour, joy_t::int_t));
		M.push(make_token(local_time.tm_min, joy_t::int_t));
		M.push(make_token(local_time.tm_sec, joy_t::int_t));
		M.push(make_token(local_time.tm_isdst, joy_t::int_t));
		M.push(make_token(local_time.tm_yday, joy_t::int_t));
		M.push(make_token(local_time.tm_wday, joy_t::int_t));
		S.push(make_token(M, joy_t::list_t));
	}

}
