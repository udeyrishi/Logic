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
#include <core/BooleanFunction.hpp>
#include <core/Utils.hpp>

using namespace Logic;

SCENARIO("A BooleanFunction contains the correct TruthTable", "[BooleanFunction]") {
    GIVEN("A BooleanFunction") {
        TruthTable table({"a", "b"});
        table[0] = true;
        BooleanFunction function(table);

        REQUIRE(function.getTruthTable()[0]);
        REQUIRE(!function.getTruthTable()[1]);

        WHEN("The BooleanFunction's table is modified") {
            THEN("The TruthTable instance passed when creation is not modified (i.e., the BooleanFunction copied the table)") {
                function.getTruthTable()[1] = true;
                REQUIRE(function.getTruthTable()[0]);
                REQUIRE(function.getTruthTable()[1]);

                REQUIRE(table[0]);
                REQUIRE(!table[1]);
                REQUIRE(&table != &function.getTruthTable());
            }
        }

        WHEN("The function is compared with an equal function") {
            TruthTable table2({"a", "b"});
            table2[0] = true;
            BooleanFunction function2(table2);
            bool result = function == function2;

            THEN("The result is true") {
                REQUIRE(result);
            }
        }

        WHEN("The function is compared with the same function but with different variables") {
            TruthTable table2({"x", "y"});
            table2[0] = true;
            BooleanFunction function2(table2);
            bool result = function == function2;

            THEN("The result is false") {
                REQUIRE(!result);
            }
        }

        WHEN("The function is compared with a different function") {
            TruthTable table2({"a", "b"});
            BooleanFunction function2(table2);

            bool result = function == function2;
            THEN("The result is false") {
                REQUIRE(!result);
            }
        }
    }
}

SCENARIO("A BooleanFunction applies UnaryOperators properly", "[BooleanFunction]") {
    GIVEN("A BooleanFunction") {
        TruthTable table({"a", "b"});
        table[0] = true;
        BooleanFunction function(table);

        WHEN("The operator Not is applied") {
            THEN("The values get inverted") {
                Not<bool> notOperator;
                BooleanFunction notted = function.operate(notOperator);
                REQUIRE(notted.getTruthTable().size() == 4);
                REQUIRE(notted.getTruthTable().getVariables().size() == 2);
                REQUIRE(!notted.getTruthTable()[0]);
                REQUIRE(notted.getTruthTable()[1]);
                REQUIRE(notted.getTruthTable()[2]);
                REQUIRE(notted.getTruthTable()[3]);
            }
        }

        WHEN("A custom UnaryOperator is applied") {
            THEN("The BooleanFunction applies the operation to all the values correctly") {
                class MyOperator : public UnaryOperator<bool> {
                public:
                    virtual bool operator()(const bool value) {
                        UNUSED(value);
                        return true;
                    }
                };

                MyOperator myOperator;
                BooleanFunction result = function.operate(myOperator);
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

SCENARIO("A BooleanFunction applies BinaryOperators properly", "[BooleanFunction]") {
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
                And<bool> andOperator;
                // !a & !b & !c
                BooleanFunction anded = function1.operate(andOperator, function2);
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
                Or<bool> orOperator;
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
                BooleanFunction ored = function1.operate(orOperator, function2);
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
                Xor<bool> xorOperator;
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
                BooleanFunction xored = function1.operate(xorOperator, function2);
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

        WHEN("A custom BinaryOperator is applied") {
            THEN("The BooleanFunction applies the operation to all the values correctly") {
                class MyOperator : public BinaryOperator<bool> {
                private:
                    int counter = 0;
                public:
                    virtual bool operator()(const bool value1, const bool value2) {
                        UNUSED(value1);
                        UNUSED(value2);
                        // operator will be called 8 times, because it first combines the tables
                        // (ignoring duplicate variables), and then collapses
                        if (counter++ < 8) {
                            return true;
                        }
                        return false;
                    }
                };

                MyOperator myOperator;
                BooleanFunction result = function1.operate(myOperator, function2);
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