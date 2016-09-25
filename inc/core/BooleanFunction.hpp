#pragma once

#include <core/TruthTable.hpp>
#include <core/Operators.hpp>
#include <stdint.h>
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

    BooleanFunction operate(const UnaryOperator<bool> &_operator) const;
    BooleanFunction operate(const BinaryOperator<bool> &_operator, BooleanFunction &other) const;

private:
    TruthTable table;

    TruthTable combineTables(const BinaryOperator<bool> &_operator, BooleanFunction &other) const;
    static TruthTable combineColumnsWithSameVariables(const TruthTable &table);
};

ostream &operator<<(ostream &os, const BooleanFunction &function);
}