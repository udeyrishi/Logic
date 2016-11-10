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
#include <core/Utils.hpp>

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
            UnaryOperator *_operator = createUnaryOperatorWithSymbol("!");
            REQUIRE(dynamic_cast<Not*>(_operator));
            delete _operator;
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with '&'") {
        THEN("The correct And operator object is returned") {
            BinaryOperator *_operator = createBinaryOperatorWithSymbol("&");
            REQUIRE(dynamic_cast<And*>(_operator));
            delete _operator;
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with '|'") {
        THEN("The correct Or operator object is returned") {
            BinaryOperator *_operator = createBinaryOperatorWithSymbol("|");
            REQUIRE(dynamic_cast<Or*>(_operator));
            delete _operator;
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with '^'") {
        THEN("The correct Xor operator object is returned") {
            BinaryOperator *_operator = createBinaryOperatorWithSymbol("^");
            REQUIRE(dynamic_cast<Xor*>(_operator));
            delete _operator;
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used when an unknown operator") {
        THEN("invalid_argument is thrown") {
            CHECK_THROWS_AS({ createUnaryOperatorWithSymbol("^"); }, invalid_argument);
            CHECK_THROWS_AS({ createUnaryOperatorWithSymbol("&"); }, invalid_argument);
        }
    }

    WHEN("createUnaryOperatorWithSymbol() is used with an unknown operator") {
        THEN("invalid_argument is thrown") {
            CHECK_THROWS_AS({ createBinaryOperatorWithSymbol("!"); }, invalid_argument);
            CHECK_THROWS_AS({ createBinaryOperatorWithSymbol("*"); }, invalid_argument);
        }
    }
}

SCENARIO("BoolTransformationUnaryOperators work", "[Not]") {
    GIVEN("A BooleanFunction") {
        TruthTable table({"a", "b"});
        table[0] = true;
        BooleanFunction function(table);

        WHEN("The operator Not is applied") {
            THEN("The values get inverted") {
                Not notOperator;
                BooleanFunction notted = notOperator(function);
                REQUIRE(notted.getTruthTable().size() == 4);
                REQUIRE(notted.getTruthTable().getVariables().size() == 2);
                REQUIRE(!notted.getTruthTable()[0]);
                REQUIRE(notted.getTruthTable()[1]);
                REQUIRE(notted.getTruthTable()[2]);
                REQUIRE(notted.getTruthTable()[3]);
            }
        }

        WHEN("A custom BoolTransformationUnaryOperator is applied") {
            THEN("The resulting function is the original function with the operator applied to all the values") {
                class MyOperator : public BoolTransformationUnaryOperator {
                private:
                    virtual bool operate(const bool value) const {
                        UNUSED(value);
                        return true;
                    }
                };

                MyOperator myOperator;
                BooleanFunction result = myOperator(function);
                REQUIRE(result.getTruthTable().size() == 4);
                REQUIRE(result.getTruthTable().getVariables().size() == 2);
                REQUIRE(result.getTruthTable()[0]);
                REQUIRE(result.getTruthTable()[1]);
                REQUIRE(result.getTruthTable()[2]);
                REQUIRE(result.getTruthTable()[3]);
            }
        }
    }
}

SCENARIO("CombinatoryBinaryOperator work", "[Operator]") {
    GIVEN("Two BooleanFunction") {
        // !a & !b
        TruthTable table1({"a", "b"});
        table1[0] = true;
        BooleanFunction function1(table1);
        // !b & !c
        TruthTable table2({"b", "c"});
        table2[0] = true;
        BooleanFunction function2(table2);

        WHEN("The operator And is applied") {
            THEN("The values get And-ed") {
                And andOperator;
                // !a & !b & !c
                BooleanFunction anded = andOperator(function1, function2);
                REQUIRE(anded.getTruthTable().size() == 8);
                REQUIRE(anded.getTruthTable().getVariables().size() == 3);
                REQUIRE(anded.getTruthTable()[0]);
                REQUIRE(!anded.getTruthTable()[1]);
                REQUIRE(!anded.getTruthTable()[2]);
                REQUIRE(!anded.getTruthTable()[3]);
                REQUIRE(!anded.getTruthTable()[4]);
                REQUIRE(!anded.getTruthTable()[5]);
                REQUIRE(!anded.getTruthTable()[6]);
                REQUIRE(!anded.getTruthTable()[7]);
            }
        }

        WHEN("The operator Or is applied") {
            THEN("The values get Or-ed") {
                Or orOperator;
                /*
                (!a & !b) | (!b & !c)
                   c b a
                   0 0 0 1
                   0 0 1 1
                   0 1 0 0
                   0 1 1 0
                   1 0 0 1
                   1 0 1 0
                   1 1 0 0
                   1 1 1 0
                */
                BooleanFunction ored = orOperator(function1, function2);
                REQUIRE(ored.getTruthTable().size() == 8);
                REQUIRE(ored.getTruthTable().getVariables().size() == 3);
                REQUIRE(ored.getTruthTable()[0]);
                REQUIRE(ored.getTruthTable()[1]);
                REQUIRE(!ored.getTruthTable()[2]);
                REQUIRE(!ored.getTruthTable()[3]);
                REQUIRE(ored.getTruthTable()[4]);
                REQUIRE(!ored.getTruthTable()[5]);
                REQUIRE(!ored.getTruthTable()[6]);
                REQUIRE(!ored.getTruthTable()[7]);
            }
        }

        WHEN("The operator Xor is applied") {
            THEN("The values get Xor-ed") {
                Xor xorOperator;
                /*
                (!a & !b) ^ (!b & !c)
                   c b a
                   0 0 0 0
                   0 0 1 1
                   0 1 0 0
                   0 1 1 0
                   1 0 0 1
                   1 0 1 0
                   1 1 0 0
                   1 1 1 0
                */
                BooleanFunction xored = xorOperator(function1, function2);
                REQUIRE(xored.getTruthTable().size() == 8);
                REQUIRE(xored.getTruthTable().getVariables().size() == 3);
                REQUIRE(!xored.getTruthTable()[0]);
                REQUIRE(xored.getTruthTable()[1]);
                REQUIRE(!xored.getTruthTable()[2]);
                REQUIRE(!xored.getTruthTable()[3]);
                REQUIRE(xored.getTruthTable()[4]);
                REQUIRE(!xored.getTruthTable()[5]);
                REQUIRE(!xored.getTruthTable()[6]);
                REQUIRE(!xored.getTruthTable()[7]);
            }
        }

        WHEN("A custom CombinatoryBinaryOperator is applied") {
            THEN("The result is correct") {
                class MyOperator : public CombinatoryBinaryOperator {
                private:
                    int counter = 0;
                    virtual bool operate(const bool value1, const bool value2) const {
                        UNUSED(value1);
                        UNUSED(value2);
                        // operator will be called 8 times, because it first combines the tables
                        // (ignoring duplicate variables), and then collapses
                        // Safe to const_cast, because this is a weird case
                        if (const_cast<MyOperator*>(this)->counter++ < 8) {
                            return true;
                        }
                        return false;
                    }
                };

                MyOperator myOperator;
                BooleanFunction result = myOperator(function1, function2);
                REQUIRE(result.getTruthTable().size() == 8);
                REQUIRE(result.getTruthTable().getVariables().size() == 3);
                REQUIRE(result.getTruthTable()[0]);
                REQUIRE(result.getTruthTable()[1]);
                REQUIRE(result.getTruthTable()[2]);
                REQUIRE(result.getTruthTable()[3]);
                REQUIRE(!result.getTruthTable()[4]);
                REQUIRE(!result.getTruthTable()[5]);
                REQUIRE(!result.getTruthTable()[6]);
                REQUIRE(!result.getTruthTable()[7]);
            }
        }
    }
}
