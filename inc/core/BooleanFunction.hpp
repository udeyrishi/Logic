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

#pragma once

#include <core/TruthTable.hpp>
#include <core/Operators.hpp>
#include <utility>
#include <vector>
#include <string>

namespace Logic {
class BooleanFunction {
public:
    BooleanFunction(const TruthTable &table);
    BooleanFunction(const bool constValue);
    BooleanFunction(const BooleanFunction &rhs);
    ~BooleanFunction();
    BooleanFunction &operator=(const BooleanFunction &rhs);

    bool hasTruthTable() const;
    TruthTable &getTruthTable();
    const TruthTable &getTruthTable() const;

    bool isConstant() const;
    bool &getConstantValue();
    bool getConstantValue() const;

    // Operates on all the values in table individually, if there's a table
    // Or operates on the constant value if a constant-value Boolean function
    BooleanFunction operate(UnaryOperator<bool> &_operator) const;
    BooleanFunction operate(BinaryOperator<bool> &_operator, const BooleanFunction &other) const;

    // May not operate on the individual values, as the logic maybe more complicated
    BooleanFunction operate(UnaryOperator<BooleanFunction> &_operator) const;
    BooleanFunction operate(BinaryOperator<BooleanFunction> &_operator, const BooleanFunction &other) const;

private:
    TruthTable *table;
    bool *constValue;

    void init(const BooleanFunction &rhs);
    void destroy();

    TruthTableBuilder combineTables(BinaryOperator<bool> &_operator, const BooleanFunction &other) const;
    static TruthTable combineColumnsWithSameVariables(const TruthTableBuilder &rawBuilder);
};

ostream &operator<<(ostream &os, const BooleanFunction &function);
bool operator==(const BooleanFunction &left, const BooleanFunction &right);

}
