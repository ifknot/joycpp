/**
* Behaviour Driven Development
*/
#include "catch2.h"

#include <iostream>

#include "joy_types.h"

using namespace joy;

SCENARIO("joy types", "[joy_types]") {
	GIVEN("test values") {
		pattern_t T = "true";
		pattern_t F = "false";

		pattern_t I1 = "+1";
		pattern_t I2 = "-100";
		pattern_t I3 = "0";
		pattern_t I4 = "65535";
		pattern_t I5 = "+03";
		pattern_t I6 = "+";

		pattern_t D1 = "3.0";
		pattern_t D2 = "+3.0";
		pattern_t D3 = "-3.0";
		pattern_t D4 = "3.23e23";
		pattern_t D5 = "-4.7e+9";
		pattern_t D6 = "-.2E-4";
		pattern_t D7 = "-7.6603";
		pattern_t D8 = "+0003.14";
		pattern_t D9 = "37.e88";

		WHEN("booleans") {
			REQUIRE(is_joy_bool(T));
			REQUIRE(is_joy_bool(F));
			REQUIRE(!is_joy_bool(I1));
		}

		WHEN("integers") {
			REQUIRE(is_joy_int(I1));
		}

	}
}


/*
SCENARIO("portal", "[portal]") {
	GIVEN() {
		REQUIRE();
		WHEN() {
			THEN() {
				REQUIRE();
			}
		}
	}
}
*/