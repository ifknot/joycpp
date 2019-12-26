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
		pattern_t I5 = "+003";		//should fail
		pattern_t I6 = "+";			//should fail

		pattern_t D1 = "3.0";
		pattern_t D2 = "+3.0";
		pattern_t D3 = "-3.0";
		pattern_t D4 = "3.23e23";
		pattern_t D5 = "-4.7e+9";
		pattern_t D6 = "-.2E-4";
		pattern_t D7 = "-7.6603";
		pattern_t D8 = "+0003.14";	//should fail
		pattern_t D9 = "37.e88";	//should fail

		pattern_t S1 = "'A 'B 'C ";

		pattern_t L1 = "[ ]";
		pattern_t L2 = "[ [ ] ]";
		pattern_t L3 = "[ 'A 'B 'C ]";
		pattern_t L4 = "[ 'A 'B 'C }";
		pattern_t L5 = "{ 'A 'B 'C ]";
		pattern_t L6 = "[ dup * ]";			//shoulde fail is quote
		pattern_t L7 = "[ 'A 'B 'C ] ]";	//should fail broken nested
		pattern_t L8 = "[ [ 'A 'B 'C ]";	//should fail broken nested

		pattern_t Q1 = "[ dup ]";
		pattern_t Q2 = "[ ]";				//are empty quotes permitted?

		WHEN("booleans") {
			REQUIRE(is_joy_bool(T));
			REQUIRE(is_joy_bool(F));
			REQUIRE(!is_joy_bool(I1));
			REQUIRE(!is_joy_bool(I1));
			REQUIRE(!is_joy_bool(I2));
			REQUIRE(!is_joy_bool(I3));
			REQUIRE(!is_joy_bool(I4));
		}

		WHEN("integers") {
			REQUIRE(!is_joy_int(T));
			REQUIRE(!is_joy_int(F));
			REQUIRE(is_joy_int(I1));
			REQUIRE(is_joy_int(I2));
			REQUIRE(is_joy_int(I3));
			REQUIRE(is_joy_int(I4));
			//TODO: fix these
			//REQUIRE(!is_joy_int(I5));
			//REQUIRE(!is_joy_int(I6));
			REQUIRE(!is_joy_int(D1));
			REQUIRE(!is_joy_int(D2));
			REQUIRE(!is_joy_int(D3));
			REQUIRE(!is_joy_int(D4));
			REQUIRE(!is_joy_int(D5));
			REQUIRE(!is_joy_int(D6));
			REQUIRE(!is_joy_int(D7));
			REQUIRE(!is_joy_int(D8));
			REQUIRE(!is_joy_int(D9));
			
		}

		WHEN("doubles") {
			REQUIRE(!is_joy_double(T));
			REQUIRE(!is_joy_double(F));
			REQUIRE(!is_joy_double(I1));
			REQUIRE(!is_joy_double(I2));
			REQUIRE(!is_joy_double(I3));
			REQUIRE(!is_joy_double(I4));
			REQUIRE(!is_joy_double(I5));
			REQUIRE(!is_joy_double(I6));
			REQUIRE(is_joy_double(D1));
			REQUIRE(is_joy_double(D2));
			REQUIRE(is_joy_double(D3));
			REQUIRE(is_joy_double(D4));
			REQUIRE(is_joy_double(D5));
			REQUIRE(is_joy_double(D6));
			REQUIRE(is_joy_double(D7));
			REQUIRE(!is_joy_double(D8));
			REQUIRE(!is_joy_double(D9));
		}

		WHEN("sigils") {
			//REQUIRE(strip_sigils(L1, LIST_OPEN, LIST_CLOSE) == "");
			REQUIRE(strip_sigils(L3, LIST_OPEN, LIST_CLOSE) == S1);
		}

		WHEN("lists") {
			REQUIRE(is_joy_list(L1));
			REQUIRE(is_joy_list(L2));
			REQUIRE(is_joy_list(L3));
			REQUIRE(!is_joy_list(L4));
			REQUIRE(!is_joy_list(L5));
			//REQUIRE(!is_joy_list(L6));
			//REQUIRE(!is_joy_list(L7));
			//REQUIRE(!is_joy_list(L8));
		}

		WHEN("quotes") {
			REQUIRE(is_joy_quote(Q1));
			REQUIRE(!is_joy_quote(Q2));
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