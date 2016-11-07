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
#include <core/Exceptions.hpp>

using namespace Logic;

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
                condition.addCondition("a", true);
                condition.addCondition("ball", true);
                condition.process();
                REQUIRE(!condition.hasCollapsedToConstant());
                TruthTable result = condition.getTruthTable();

                REQUIRE(result.size() == 2);
                REQUIRE(result.getVariables().size() == 1);
                REQUIRE(result.getVariables()[0] == "cat");
                REQUIRE(result[0] == true);
                REQUIRE(result[1] == true);
            }
        }

        WHEN("You apply a condition on a variable that does not exist") {
            THEN("invalid_argument exception is thrown") {
                CHECK_THROWS_AS({ table.conditionBuilder().addCondition("b", true); }, invalid_argument);
            }
        }

        WHEN("You apply a condition on a variable multiple times") {
            THEN("The values overwrite, and the last one is kept") {
                TruthTableCondition condition = table.conditionBuilder();
                condition.addCondition("a", true);
                condition.addCondition("a", false);
                condition.process();
                REQUIRE(!condition.hasCollapsedToConstant());
                TruthTable result = condition.getTruthTable();
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

        WHEN("You apply a condition to all the variables") {
            THEN("IllegalStateException is thrown when creating the resulting table") {
                TruthTableCondition condition = table.conditionBuilder();
                condition.addCondition("a", true);
                condition.addCondition("ball", true);
                condition.addCondition("cat", false);
                condition.process();

                REQUIRE(condition.hasCollapsedToConstant());
                REQUIRE(!condition.getConstant());
                CHECK_THROWS_AS({ condition.getTruthTable(); }, IllegalStateException);
            }
        }

        WHEN("You apply a condition after processing previously applied conditions") {
            TruthTableCondition condition = table.conditionBuilder();
            table[1] = true;
            condition.addCondition("a", true);
            condition.addCondition("ball", false);
            condition.process();
            REQUIRE(!condition.hasCollapsedToConstant());
            condition.addCondition("cat", false);
            REQUIRE(!condition.hasCollapsedToConstant());

            THEN("The new condition is added after a new process call") {
                condition.process();
                REQUIRE(condition.hasCollapsedToConstant());
                CHECK_THROWS_AS({ condition.getTruthTable(); }, IllegalStateException);
                REQUIRE(condition.getConstant());
            }
        }

        WHEN("You change some values in the truthtable") {
            table[0] = true;
            table[4] = true;

            THEN("The table returns the correct minterms") {
                vector<TruthTableUInt> minterms = table.getMinterms();
                REQUIRE(minterms.size() == 2);
                REQUIRE(minterms[0] == 0);
                REQUIRE(minterms[1] == 4);
            }

            THEN("The table returns the correct maxterms") {
                vector<TruthTableUInt> maxterms = table.getMaxterms();
                REQUIRE(maxterms.size() == 6);
                REQUIRE(maxterms[0] == 1);
                REQUIRE(maxterms[1] == 2);
                REQUIRE(maxterms[2] == 3);
                REQUIRE(maxterms[3] == 5);
                REQUIRE(maxterms[4] == 6);
                REQUIRE(maxterms[5] == 7);
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

SCENARIO("A TruthTable equality operator works properly", "[TruthTable]") {
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
