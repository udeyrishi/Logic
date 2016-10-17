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
#include <core/BooleanFunctionParser.hpp>

using namespace Logic;

SCENARIO("A BooleanFunctionAccumulator does not modify Boolean functions without any operators", "[BooleanFunctionParser]") {
    GIVEN("A BooleanFunctionAccumulator instance") {
        BooleanFunctionAccumulator accumulator;

        REQUIRE(!accumulator.canBePopped());

        WHEN("A Boolean function is pushed") {
            THEN("The same function is popped") {
                TruthTable table({"a", "b"});
                table[3] = true;
                BooleanFunction function(table);
                accumulator.push(function);
                REQUIRE(accumulator.canBePopped());
                BooleanFunction popped = accumulator.pop();
                REQUIRE(popped == function);
                REQUIRE(!accumulator.canBePopped());
            }
        }
    }
}

SCENARIO("A BooleanFunctionAccumulator applies a unary operator pushed on top of a BooleanFunction", "[BooleanFunctionAccumulator]") {
    GIVEN("A BooleanFunctionAccumulator with a function pushed in it") {
        BooleanFunctionAccumulator accumulator;
        TruthTable table({"a", "b"});
        table[3] = true;
        BooleanFunction function(table);
        accumulator.push(function);

        WHEN("A unary operator is pushed, and then the result is popped") {
            UnaryOperator<bool> *_operator = new Not<bool>();
            accumulator.push(*_operator);
            delete _operator;
            BooleanFunction result = accumulator.pop();

            THEN("The result = operator(original function)") {
                UnaryOperator<bool> *_operator = new Not<bool>();
                BooleanFunction notted = function.operate(*_operator);
                delete _operator;
                REQUIRE(notted == result);
                REQUIRE(!accumulator.canBePopped());
            }
        }
    }
}

SCENARIO("A BooleanFunctionAccumulator applies a binary operator", "[BooleanFunctionAccumulator]") {
    GIVEN("A BooleanFunctionAccumulator with two functions pushed in it") {
        BooleanFunctionAccumulator accumulator;
        TruthTable table({"a", "b"});
        table[3] = true;
        BooleanFunction function(table);
        accumulator.push(function);
        table = TruthTable({"b", "c"});
        table[0] = true;
        BooleanFunction function2(table);
        accumulator.push(function2);

        WHEN("A binary operator is pushed, and then the result is popped") {
            BinaryOperator<bool> *_operator = new And<bool>();
            accumulator.push(*_operator);
            delete _operator;
            BooleanFunction result = accumulator.pop();

            THEN("The result = operator(original functions)") {
                BinaryOperator<bool> *_operator = new And<bool>();
                BooleanFunction anded = function.operate(*_operator, function2);
                delete _operator;
                REQUIRE(anded == result);
                REQUIRE(!accumulator.canBePopped());
            }
        }
    }
}
