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
#include <core/TruthTable.hpp>
#include <sstream>
#include <Exceptions.hpp>

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

// TruthTableUInt is just a typedef right now, so tests is an overkill. Add if this is not true in the future

SCENARIO("A TruthTable stores the variable and data properly", "[TruthTable, TruthTableCondition]") {
    GIVEN("A 3-variable TruthTable") {
        TruthTable table({"a", "ball", "cat"});

        REQUIRE(table.size() == 8);
        REQUIRE(table.getVariables().size() == 3);
        REQUIRE(table.getVariables()[0] == "a");
        REQUIRE(table.getVariables()[1] == "ball");
        REQUIRE(table.getVariables()[2] == "cat");

        REQUIRE(TruthTable::getVariableValueInLine(0, 0) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(1, 0) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(2, 0) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(3, 0) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(4, 0) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(0, 1) == 1);
        REQUIRE(TruthTable::getVariableValueInLine(1, 1) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(2, 1) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(3, 1) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(4, 1) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(0, 2) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(1, 2) == 1);
        REQUIRE(TruthTable::getVariableValueInLine(2, 2) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(3, 2) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(4, 2) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(0, 6) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(1, 6) == 1);
        REQUIRE(TruthTable::getVariableValueInLine(2, 6) == 1);
        REQUIRE(TruthTable::getVariableValueInLine(3, 6) == 0);
        REQUIRE(TruthTable::getVariableValueInLine(4, 6) == 0);

        WHEN("The [] operator is used within table's range") {
            THEN("The table's value can be set and retrieved at that index") {
                for (TruthTableUInt i = 0; i < table.size(); ++i) {
                    REQUIRE(!table[i]);
                }
                table[5] = !table[5];
                for (TruthTableUInt i = 0; i < table.size(); ++i) {
                    if (i == 5) {
                        REQUIRE(table[i]);
                    } else {
                        REQUIRE(!table[i]);
                    }
                }
                table[4] = !table[5];
                for (TruthTableUInt i = 0; i < table.size(); ++i) {
                    if (i == 5) {
                        REQUIRE(table[i]);
                    } else {
                        REQUIRE(!table[i]);
                    }
                }
            }
        }

        WHEN("The [] operator is used outside of table's range") {
            THEN("out_of_range exception is thrown") {
                CHECK_THROWS_AS({ table[8]; }, out_of_range);
                CHECK_THROWS_AS({ table[52]; }, out_of_range);
                CHECK_THROWS_AS({ table[8] = true; }, out_of_range);
            }
        }

        WHEN("You apply a condition on a variable") {
            THEN("The correct resulting table is generated") {
                // a & b;
                table[3] = true;
                table[7] = true;

                TruthTableCondition condition = table.conditionBuilder();
                condition.when("a", true);
                condition.when("ball", true);
                TruthTable result = condition.then();

                REQUIRE(result.size() == 2);
                REQUIRE(result.getVariables().size() == 1);
                REQUIRE(result.getVariables()[0] == "cat");
                REQUIRE(result[0] == true);
                REQUIRE(result[1] == true);
            }
        }

        WHEN("You apply a condition on a variable that does not exist") {
            THEN("invalid_argument exception is thrown") {
                CHECK_THROWS_AS({ table.conditionBuilder().when("b", true); }, invalid_argument);
            }
        }

        WHEN("You apply a condition on a variable multiple times") {
            THEN("The values overwrite, and the last one is kept") {
                TruthTableCondition condition = table.conditionBuilder();
                condition.when("a", true);
                condition.when("a", false);
                TruthTable result = condition.then();
                REQUIRE(result.size() == 4);
                REQUIRE(result.getVariables().size() == 2);
                REQUIRE(result.getVariables()[0] == "ball");
                REQUIRE(result.getVariables()[1] == "cat");
                REQUIRE(result[0] == false);
                REQUIRE(result[1] == false);
                REQUIRE(result[2] == false);
                REQUIRE(result[3] == false);
            }
        }

        // TODO: This is the current behaviour. Maybe this can be better?
        WHEN("You apply a condition to all the variables") {
            THEN("IllegalTruthTableException is thrown when creating the resulting table") {
                TruthTableCondition condition = table.conditionBuilder();
                condition.when("a", true);
                condition.when("ball", true);
                condition.when("cat", false);
                CHECK_THROWS_AS({ condition.then(); }, IllegalTruthTableException);
            }
        }
    }

    WHEN("You try to create a truthtable with no variables") {
        THEN("invalid_argument exception is thrown") {
            CHECK_THROWS_AS({ TruthTable({}); }, invalid_argument);
        }
    }

    WHEN("You try to create a truthtable with > 64 variables") {
        THEN("invalid_argument exception is thrown") {
            vector<string> vars;
            for (int i = 0; i < 65; ++i) {
                vars.push_back(string(1, 'a' + (char)i));
            }
            CHECK_THROWS_AS({ TruthTable x(vars); }, invalid_argument);
            vars.push_back("hello");
            CHECK_THROWS_AS({ TruthTable x(vars); }, invalid_argument);
        }
    }

    WHEN("You try to create a truthtable with duplicate variables") {
        THEN("invalid_argument exception is thrown") {
            CHECK_THROWS_AS({ TruthTable({"x", "hello", "x"}); }, invalid_argument);
        }
    }
}

SCENARIO("A TruthTable equiality operator works properly", "[TruthTable]") {
    GIVEN("A TruthTable") {
        TruthTable table({"x", "y"});
        table[3] = true;

        WHEN("The TruthTable is compared with an equal table") {
            TruthTable table2({"x", "y"});
            table2[3] = true;
            bool result = table == table2;

            THEN("The result is true") {
                REQUIRE(result);
            }
        }

        WHEN("The TruthTable is compared with the same table but with different variables") {
            TruthTable table2({"a", "b"});
            table2[3] = true;
            bool result = table == table2;

            THEN("The result is false") {
                REQUIRE(!result);
            }
        }

        WHEN("The TruthTable is compared with a different table") {
            TruthTable table2({"a", "b"});
            bool result = table == table2;
            THEN("The result is false") {
                REQUIRE(!result);
            }
        }
    }
}

SCENARIO("A TruthTableBuilder builds a TruthTable", "[TruthTableBuilder]") {
    GIVEN("A TruthTableBuilder") {
        TruthTableBuilder builder;

        WHEN("The variables and values are set") {
            THEN("The accessors return correct values") {
                // duplicates allowed when setting
                builder.setVariables({"a", "a"});
                builder.set(0, true);
                builder.set(1, false);
                builder.set(2, true);
                builder.set(3, false);
                REQUIRE(builder.getVariables().size() == 2);
                REQUIRE(builder.getVariables()[0] == "a");
                REQUIRE(builder.getVariables()[1] == "a");
                REQUIRE(builder.getValue(0));
                REQUIRE(!builder.getValue(1));
                REQUIRE(builder.getValue(2));
                REQUIRE(!builder.getValue(3));
                REQUIRE(builder.tentativeSize() == 4);
            }
        }

        WHEN("The variables and values are all set") {
            THEN("The TruthTable is properly built") {
                builder.setVariables({"a", "b"});
                builder.set(0, true);
                builder.set(1, false);
                builder.set(2, true);
                builder.set(3, false);
                TruthTable table = builder.build();
                REQUIRE(table.size() == 4);
                REQUIRE(table.getVariables().size() == 2);
                REQUIRE(table.getVariables()[0] == "a");
                REQUIRE(table.getVariables()[1] == "b");
                REQUIRE(table[0]);
                REQUIRE(!table[1]);
                REQUIRE(table[2]);
                REQUIRE(!table[3]);
            }
        }

        WHEN("The variables are left unset") {
            THEN("IllegalTruthTableException is thrown") {
                builder.set(0, true);
                builder.set(1, false);
                CHECK_THROWS_AS({ builder.build(); }, IllegalTruthTableException);
            }
        }

        WHEN("The values set != 2**numVariables") {
            THEN("IllegalTruthTableException is thrown") {
                builder.setVariables({"a"});
                builder.set(0, true);
                builder.set(1, false);
                builder.set(2, false);
                builder.set(3, false);
                CHECK_THROWS_AS({ builder.build(); }, IllegalTruthTableException);
            }
        }

        WHEN("The values for the same index are set multiple times") {
            THEN("The values are overwrite, and the last value is kept") {
                builder.setVariables({"a"});
                builder.set(0, true);
                builder.set(1, false);
                builder.set(1, true);
                TruthTable table = builder.build();
                REQUIRE(table.size() == 2);
                REQUIRE(table.getVariables().size() == 1);
                REQUIRE(table.getVariables()[0] == "a");
                REQUIRE(table[0]);
                REQUIRE(table[1]);
            }
        }

        WHEN("The variables are set multiple times") {
            THEN("The variables are overwrite, and the last ones set are kept") {
                builder.setVariables({"a"});
                builder.set(0, true);
                builder.set(1, false);
                builder.setVariables({"a", "b"});
                CHECK_THROWS_AS({ builder.build(); }, IllegalTruthTableException);
                builder.set(2, false);
                builder.set(3, false);
                TruthTable table = builder.build();
                REQUIRE(table.size() == 4);
                REQUIRE(table.getVariables().size() == 2);
                REQUIRE(table.getVariables()[0] == "a");
                REQUIRE(table.getVariables()[1] == "b");
                REQUIRE(table[0]);
                REQUIRE(!table[1]);
                REQUIRE(!table[2]);
                REQUIRE(!table[3]);
            }
        }
    }
}
