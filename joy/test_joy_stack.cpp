/**
* Behaviour Driven Development
*/
#include "catch2.h"

#include "joy_stack.h"

SCENARIO("joy_stack", "[joy_stack]") {

	using tok = joy::token_t;

	GIVEN("an empty joy stack") {

		joy::joy_stack s;

		REQUIRE(s.size() == 0);

		WHEN("pop empty stack") {
			s.pop();
			THEN("do nothing") {
				REQUIRE(s.size() == 0);
			}
		}

		WHEN("dup empty stack") {
			s.dup();
			THEN("do nothing") {
				REQUIRE(s.size() == 0);
			}
		}
	}

	GIVEN("a stack c b a|") {

		joy::joy_stack s;

		s.push(tok("c", joy::joy_t::char_t));
		s.push(tok("b", joy::joy_t::char_t));
		s.push(tok("a", joy::joy_t::char_t));

		REQUIRE(s.size() == 3);
		REQUIRE(s.sat(0).first == "a");
		REQUIRE(s.sat(1).first == "b");
		REQUIRE(s.sat(2).first == "c");

		WHEN("dup") {
			s.dup();
			THEN("bcaa") {
				REQUIRE(s.size() == 4);
				REQUIRE(s.sat(0).first == "a");
				REQUIRE(s.sat(1).first == "a");
				REQUIRE(s.sat(2).first == "b");
				REQUIRE(s.sat(3).first == "c");
			}
		}


		WHEN("pop") {
			s.pop();
			THEN("bc") {
				REQUIRE(s.size() == 2);
				REQUIRE(s.sat(0).first == "b");
				REQUIRE(s.sat(1).first == "c");
			}
		}

		WHEN("dupd") {
			s.dupd();
			THEN("cbba") {
				REQUIRE(s.size() == 4);
				REQUIRE(s.sat(0).first == "a");
				REQUIRE(s.sat(1).first == "b");
				REQUIRE(s.sat(2).first == "b");
				REQUIRE(s.sat(3).first == "c");
			}
		}

		WHEN("popd") {
			s.popd();
			THEN("ac") {
				REQUIRE(s.size() == 2);
				REQUIRE(s.sat(0).first == "a");
				REQUIRE(s.sat(1).first == "c");
			}
		}

		WHEN("pop2") {
			s.pop2();
			THEN("c") {
				REQUIRE(s.size() == 1);
				REQUIRE(s.sat(0).first == "c");
			}
		}

		WHEN("unit") {
			s.unit();
			THEN("cb[a]") {
				REQUIRE(s.size() == 3);
				REQUIRE(s.sat(0).first == "[ a ]");
				REQUIRE(s.sat(1).first == "b");
				REQUIRE(s.sat(2).first == "c");
			}
		}

		WHEN("swap") {
			s.swap();
			THEN("cab") {
				REQUIRE(s.size() == 3);
				REQUIRE(s.sat(0).first == "b");
				REQUIRE(s.sat(1).first == "a");
				REQUIRE(s.sat(2).first == "c");
			}
		}

		WHEN("swapd") {
			s.swapd();
			THEN("bca") {
				REQUIRE(s.size() == 3);
				REQUIRE(s.sat(0).first == "a");
				REQUIRE(s.sat(1).first == "c");
				REQUIRE(s.sat(2).first == "b");
			}
		}

		WHEN("rotate") {
			s.rotate();
			THEN("abc") {
				REQUIRE(s.size() == 3);
				REQUIRE(s.sat(0).first == "c");
				REQUIRE(s.sat(1).first == "b");
				REQUIRE(s.sat(2).first == "a");
			}
		}

		WHEN("rollup") {
			s.rollup();
			THEN("acb") {
				REQUIRE(s.size() == 3);
				REQUIRE(s.sat(0).first == "b");
				REQUIRE(s.sat(1).first == "c");
				REQUIRE(s.sat(2).first == "a");
			}
		}

		WHEN("rolldown") {
			s.rolldown();
			THEN("bac") {
				REQUIRE(s.size() == 3);
				REQUIRE(s.sat(0).first == "c");
				REQUIRE(s.sat(1).first == "a");
				REQUIRE(s.sat(2).first == "b");
			}
		}

		WHEN("newstack") {
			s.newstack();
			THEN("empty") {
				REQUIRE(s.size() == 0);
				REQUIRE(s.empty());
			}
		}

		WHEN("stack") {
			s.stack();
			THEN("quoted stack") {
				REQUIRE(s.size() == 4);
				REQUIRE(s.sat(0).first == "[ a b c ]");
				REQUIRE(s.sat(1).first == "a");
				REQUIRE(s.sat(2).first == "b");
				REQUIRE(s.sat(3).first == "c");
			}
		}

		WHEN("unstack") {
			s.push(joy::make_quoted_token("[ d e f ]"));
			s.unstack();
			THEN("new stack from quote") {
				REQUIRE(s.size() == 3);
				REQUIRE(s.sat(0).first == "d");
				REQUIRE(s.sat(1).first == "e");
				REQUIRE(s.sat(2).first == "f");
			}
		}

		s.dump();

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