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
