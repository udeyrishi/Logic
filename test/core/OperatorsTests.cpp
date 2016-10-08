/**
 Copyright 2016 Udey Rishi

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include <catch.hpp>
#include <core/Operators.hpp>

using namespace Logic;

SCENARIO("Operator factories return correct operators") {
    WHEN("isKnownUnaryOperator() is used with a known/unknown unary operator") {
        THEN("The correct result is returned") {
            REQUIRE(isKnownUnaryOperator("!"));
            REQUIRE(!isKnownUnaryOperator("  !  "));
            REQUIRE(!isKnownUnaryOperator("@"));
            REQUIRE(!isKnownUnaryOperator("#"));
            REQUIRE(!isKnownUnaryOperator("$"));
            REQUIRE(!isKnownUnaryOperator("%"));
            REQUIRE(!isKnownUnaryOperator("^"));
            REQUIRE(!isKnownUnaryOperator("&"));
            REQUIRE(!isKnownUnaryOperator("*"));
            REQUIRE(!isKnownUnaryOperator("~"));
            REQUIRE(!isKnownUnaryOperator("|"));
        }
    }

    WHEN("isKnownBinaryOperator() is used with a known/unknown binary operator") {
        THEN("The correct result is returned") {
            REQUIRE(!isKnownBinaryOperator("!"));
            REQUIRE(!isKnownBinaryOperator("@"));
            REQUIRE(!isKnownBinaryOperator("#"));
            REQUIRE(!isKnownBinaryOperator("$"));
            REQUIRE(!isKnownBinaryOperator("%"));
            REQUIRE(!isKnownBinaryOperator("*"));
            REQUIRE(!isKnownBinaryOperator("~"));

            REQUIRE(isKnownBinaryOperator("|"));
            REQUIRE(!isKnownBinaryOperator("  |  "));
            REQUIRE(isKnownBinaryOperator("^"));
            REQUIRE(!isKnownBinaryOperator(" ^  "));
            REQUIRE(isKnownBinaryOperator("&"));
            REQUIRE(!isKnownBinaryOperator("  &  "));
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with a known unary operator") {
        THEN("The correct operator object is returned") {
            UnaryOperator<bool> *_operator = createUnaryOperatorWithSymbol<bool>("!");
            REQUIRE(_operator);
            REQUIRE(!(*_operator)(true));
            REQUIRE((*_operator)(false));
            delete _operator;
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with '&'") {
        THEN("The correct And operator object is returned") {
            BinaryOperator<bool> *_operator = createBinaryOperatorWithSymbol<bool>("&");
            REQUIRE(_operator);
            REQUIRE(!(*_operator)(false, false));
            REQUIRE(!(*_operator)(false, true));
            REQUIRE(!(*_operator)(true, false));
            REQUIRE((*_operator)(true, true));
            delete _operator;
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with '|'") {
        THEN("The correct Or operator object is returned") {
            BinaryOperator<bool> *_operator = createBinaryOperatorWithSymbol<bool>("|");
            REQUIRE(_operator);
            REQUIRE(!(*_operator)(false, false));
            REQUIRE((*_operator)(false, true));
            REQUIRE((*_operator)(true, false));
            REQUIRE((*_operator)(true, true));
            delete _operator;
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with '^'") {
        THEN("The correct Xor operator object is returned") {
            BinaryOperator<bool> *_operator = createBinaryOperatorWithSymbol<bool>("^");
            REQUIRE(_operator);
            REQUIRE(!(*_operator)(false, false));
            REQUIRE((*_operator)(false, true));
            REQUIRE((*_operator)(true, false));
            REQUIRE(!(*_operator)(true, true));
            delete _operator;
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used when an unknown operator") {
        THEN("invalid_argument is thrown") {
            CHECK_THROWS_AS({ createUnaryOperatorWithSymbol<bool>("^"); }, invalid_argument);
            CHECK_THROWS_AS({ createUnaryOperatorWithSymbol<bool>("&"); }, invalid_argument);
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with an unknown operator") {
        THEN("invalid_argument is thrown") {
            CHECK_THROWS_AS({ createBinaryOperatorWithSymbol<bool>("!"); }, invalid_argument);
            CHECK_THROWS_AS({ createBinaryOperatorWithSymbol<bool>("*"); }, invalid_argument);
        }
    }
}

SCENARIO("And operator works", "[And]") {
    GIVEN("An And operator object") {
        And<bool> _operator;
        REQUIRE(!_operator(false, false));
        REQUIRE(!_operator(false, true));
        REQUIRE(!_operator(true, false));
        REQUIRE(_operator(true, true));
    }
}

SCENARIO("Or operator works", "[Or]") {
    GIVEN("An Or operator object") {
        Or<bool> _operator;
        REQUIRE(!_operator(false, false));
        REQUIRE(_operator(false, true));
        REQUIRE(_operator(true, false));
        REQUIRE(_operator(true, true));
    }
}

SCENARIO("Xor operator works", "[Xor]") {
    GIVEN("An Xor operator object") {
        Xor<bool> _operator;
        REQUIRE(!_operator(false, false));
        REQUIRE(_operator(false, true));
        REQUIRE(_operator(true, false));
        REQUIRE(!_operator(true, true));
    }
}

SCENARIO("Not operator works", "[Not]") {
    GIVEN("An Not operator object") {
        Not<bool> _operator;
        REQUIRE(_operator(false));
        REQUIRE(!_operator(true));
    }
}