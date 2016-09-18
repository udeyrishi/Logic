#include <catch.hpp>
#include <core/TruthTable.hpp>
#include <sstream>

using namespace Logic;

SCENARIO("TruthTableVariablesUInt behaves like a uint with range 0 <= x <= 64", "[TruthTableVariablesUInt]") {
    GIVEN("A TruthTableVariablesUInt with some value") {
        REQUIRE(TruthTableVariablesUInt(23) == 23);
        REQUIRE((uint8_t) TruthTableVariablesUInt(0) == 0);
        REQUIRE((uint8_t) TruthTableVariablesUInt() == 0);
        REQUIRE((uint8_t) TruthTableVariablesUInt(45) == 45);
        REQUIRE((uint32_t) TruthTableVariablesUInt(45) == 45);
        REQUIRE((uint64_t) TruthTableVariablesUInt(45) == 45);
        REQUIRE((int8_t) TruthTableVariablesUInt(45) == 45);
        REQUIRE((int32_t) TruthTableVariablesUInt(45) == 45);
        REQUIRE((int64_t) TruthTableVariablesUInt(45) == 45);
        REQUIRE((int64_t) TruthTableVariablesUInt(64) == 64);
        REQUIRE(TruthTableVariablesUInt(45) >= -52);
        REQUIRE(TruthTableVariablesUInt(45) > -52);
        REQUIRE(TruthTableVariablesUInt(45) > 0);
        REQUIRE(TruthTableVariablesUInt(45) >= 0);
        REQUIRE(TruthTableVariablesUInt(45) < 46);
        REQUIRE(TruthTableVariablesUInt(45) <= 46);
        REQUIRE(TruthTableVariablesUInt(45) < 123);
        REQUIRE(TruthTableVariablesUInt(45) <= 123);
        CHECK_THROWS_AS({ TruthTableVariablesUInt x(65); }, out_of_range);
        CHECK_THROWS_AS({ TruthTableVariablesUInt x(100); }, out_of_range);
        CHECK_THROWS_AS({ TruthTableVariablesUInt x(255); }, out_of_range);

        TruthTableVariablesUInt x = 34;
        WHEN("The pre-increment operator is used") {
            THEN("Both the variable and the return value increase") {
                TruthTableVariablesUInt y = ++x;
                REQUIRE(y == 35);
                REQUIRE(x == 35);
            }

            THEN("overflow_error is thrown if the value was 64") {
                TruthTableVariablesUInt z(64);
                CHECK_THROWS_AS({ ++z; }, overflow_error);
            }
        }

        WHEN("The post-increment operator is used") {
            THEN("Only the variable and NOT the return value increases") {
                TruthTableVariablesUInt y = x++;
                REQUIRE(y == 34);
                REQUIRE(x == 35);
            }

            THEN("overflow_error is thrown if the value was 64") {
                TruthTableVariablesUInt z(64);
                CHECK_THROWS_AS({ z++; }, overflow_error);
            }
        }

        WHEN("The pre-decrement operator is used") {
            THEN("Both the variable's and the return value decrease") {
                TruthTableVariablesUInt y = --x;
                REQUIRE(y == 33);
                REQUIRE(x == 33);
            }

            THEN("underflow_error is thrown if the value was 0") {
                TruthTableVariablesUInt z(0);
                CHECK_THROWS_AS({ --z; }, underflow_error);
            }
        }

        WHEN("The post-decrement operator is used") {
            THEN("Only the variable and NOT the return value decreases") {
                TruthTableVariablesUInt y = x--;
                REQUIRE(y == 34);
                REQUIRE(x == 33);
            }

            THEN("underflow_error is thrown if the value was 0") {
                TruthTableVariablesUInt z(0);
                CHECK_THROWS_AS({ z--; }, underflow_error);
            }
        }

        WHEN("The plus-equals operator is used") {
            THEN("The value increments by the specified amount") {
                x += 5;
                REQUIRE(x == 39);
            }

            THEN("overflow_error is thrown if the resulting value > 64") {
                CHECK_THROWS_AS({ x += 31; }, overflow_error);
            }
        }

        WHEN("The minus-equals operator is used") {
            THEN("The value decrements by the specified amount") {
                x -= 5;
                REQUIRE(x == 29);
            }

            THEN("underflow_error is thrown if the resulting value < 0") {
                CHECK_THROWS_AS({ x -= 35; }, underflow_error);
            }
        }

        WHEN("The plus operator is used") {
            THEN("The value increments by the specified amount") {
                REQUIRE(x + 5 == 39);
            }

            THEN("overflow_error is thrown if the resulting value > 64") {
                CHECK_THROWS_AS({ x + 31; }, overflow_error);
            }
        }

        WHEN("The minus operator is used") {
            THEN("The value decrements by the specified amount") {
                REQUIRE(x - 5 == 29);
            }

            THEN("underflow_error is thrown if the resulting value < 0") {
                CHECK_THROWS_AS({ x - 35; }, underflow_error);
            }
        }

        WHEN("The istream is used") {
            THEN("The resulting object is created") {
                TruthTableVariablesUInt y;
                istringstream("12") >> y;
                REQUIRE(y == 12);
            }

            THEN("out_of_range error is thrown when the number is not between 0 and 64") {
                TruthTableVariablesUInt y;
                CHECK_THROWS_AS({ istringstream("-1") >> y; }, out_of_range);
                CHECK_THROWS_AS({ istringstream("65") >> y; }, out_of_range);
            }
        }

        WHEN("The ostream is used") {
            THEN("The resulting object is printed") {
                TruthTableVariablesUInt y(12);
                ostringstream oss;
                oss << y;
                REQUIRE(oss.str() == "12");
            }
        }
    }
}