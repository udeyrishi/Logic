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
    BooleanFunction(const TruthTable &table) : table(table) {
    }

    TruthTable &getTruthTable() {
        return table;
    }

    const TruthTable &getTruthTable() const {
        return table;
    }

    BooleanFunction operate(UnaryOperator<bool> &_operator) const;
    BooleanFunction operate(BinaryOperator<bool> &_operator, BooleanFunction &other) const;

private:
    TruthTable table;

    TruthTableBuilder combineTables(BinaryOperator<bool> &_operator, BooleanFunction &other) const;
    static TruthTable combineColumnsWithSameVariables(const TruthTableBuilder &rawBuilder);
};

ostream &operator<<(ostream &os, const BooleanFunction &function);
bool operator==(const BooleanFunction &left, const BooleanFunction &right);

}