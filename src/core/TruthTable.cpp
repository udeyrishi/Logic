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

#include <core/TruthTable.hpp>
#include <iostream>
#include <Exceptions.hpp>
#include <algorithm>
#include <unordered_set>

namespace Logic {
static bool isPowerOfTwo(TruthTableUInt n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

template <typename TException>
void TruthTableVariablesUInt::assertFits(const uint64_t value) {
    if (value > MAX_NUM_VARIABLES) {
        throw TException("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator++() {
    ++value;
    assertFits<overflow_error>(value);
    return *this;
}

TruthTableVariablesUInt TruthTableVariablesUInt::operator++(int) {
    TruthTableVariablesUInt temp(*this);
    ++value;
    assertFits<overflow_error>(value);
    return temp;
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator--() {
    if (value == 0) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    --value;
    return *this;
}

TruthTableVariablesUInt TruthTableVariablesUInt::operator--(int) {
    if (value == 0) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    TruthTableVariablesUInt temp(*this);
    --value;
    return temp;
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator+=(const TruthTableVariablesUInt &rhs) {
    uint32_t added = (uint32_t) value + (uint32_t) rhs.value;
    assertFits<overflow_error>(added);
    value = (uint8_t) added;
    return *this;
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator-=(const TruthTableVariablesUInt &rhs) {
    if (rhs.value > value) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    value = value - rhs.value;
    return *this;
}

bool TruthTableVariablesUInt::operator==(const TruthTableVariablesUInt &rhs) const {
    return value == rhs.value;
}

bool TruthTableVariablesUInt::operator==(const int32_t &rhs) const {
    return (int32_t) value == rhs;
}

bool TruthTableVariablesUInt::operator==(const int64_t &rhs) const {
    return (int64_t) value == rhs;
}

bool TruthTableVariablesUInt::operator==(const uint32_t &rhs) const {
    return (uint32_t) value == rhs;
}

bool TruthTableVariablesUInt::operator==(const uint64_t &rhs) const {
    return (uint64_t) value == rhs;
}

template <typename TContainer, typename TValue>
static bool containsDuplicates(const TContainer &container) {
    unordered_set<TValue> valuesSeen;
    for (const TValue &val : container) {
        if (valuesSeen.find(val) == valuesSeen.end()) {
            valuesSeen.insert(val);
        } else {
            return true;
        }
    }
    return false;
}

TruthTable::TruthTable(const vector<string> &variables) : variables(variables), values(pow(2, variables.size()), false) {
    if (variables.size() == 0 || variables.size() > MAX_NUM_VARIABLES) {
        throw invalid_argument("variables' size needs to be 0 < n <= " + to_string(MAX_NUM_VARIABLES));
    }

    if (containsDuplicates<vector<string>, string>(variables)) {
        throw invalid_argument("TruthTable cannot contain duplicate variables");
    }
}

__TruthTableValueProxy TruthTable::operator[](const TruthTableUInt index) {
    validateIndex(index);
    return __TruthTableValueProxy(index, *this);
}

bool TruthTable::operator[](const TruthTableUInt index) const {
    validateIndex(index);
    return values[index];
}

bool operator==(const TruthTable &left, const TruthTable &right) {
    return left.getVariables() == right.getVariables() &&
           left.values == right.values;
}

void TruthTable::validateIndex(const TruthTableUInt index) const {
    if (index >= size()) {
        throw out_of_range("index needs to be in range: [0, " + to_string(size() - 1) + "]");
    }
}

TruthTableCondition TruthTable::conditionBuilder() {
    return TruthTableCondition(*this);
}

void TruthTableCondition::when(const string &variable, const bool value) {
    const auto &hit = find(table.getVariables().begin(), table.getVariables().end(), variable);
    if (hit == table.getVariables().end()) {
        throw invalid_argument("variable not found in the truth table: " + variable);
    }

    TruthTableVariablesUInt position = hit - table.getVariables().begin();
    if (conditions.find(position) != conditions.end()) {
        conditions.erase(position);
    }

    conditions.insert(make_pair(position, value));
}

TruthTable TruthTableCondition::then() {
    TruthTableUInt newTable = 0;
    TruthTableBuilder builder;
    for (TruthTableUInt i = 0; i < table.size(); ++i) {
        bool allConditionsMet = true;
        for (const auto &condition : conditions) {
            if ((bool) ((i >> condition.first) & 1) != condition.second) {
                allConditionsMet = false;
                break;
            }
        }

        if (allConditionsMet) {
            builder.set(newTable++, table[i]);
        }
    }

    vector<string> newVariables;
    for (TruthTableVariablesUInt i = 0; i < table.getVariables().size(); ++i) {
        if (conditions.find(i) == conditions.end()) {
            newVariables.push_back(table.getVariables()[i]);
        }
    }
    builder.setVariables(newVariables);
    return builder.build();
}

TruthTable TruthTableBuilder::build() const {
    if (values.size() <= 1 || !isPowerOfTwo(values.size())) {
        throw IllegalTruthTableException("Number of lines needs to be a power of 2 that's greater than 1.");
    }

    if ((TruthTableUInt) pow(2, variables.size()) != values.size()) {
        throw IllegalTruthTableException("Number of lines should be 2**number of variables.");
    }

    TruthTable built(variables);
    for (TruthTableUInt i = 0; i < values.size(); ++i) {
        built[i] = values[i];
    }

    return built;
}

void TruthTableBuilder::set(TruthTableUInt lineIndex, const bool b) {
    for (TruthTableUInt i = values.size(); i <= lineIndex; ++i) {
        values.push_back(false);
    }

    values[lineIndex] = b;
}

TruthTableUInt TruthTableBuilder::tentativeSize() const {
    return (TruthTableUInt) pow(2, variables.size());
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const TruthTableVariablesUInt &rhs) {
    return lhs + (uint64_t) rhs;
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const int32_t &rhs) {
    return lhs + (int64_t) rhs;
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const int64_t &rhs) {
    int64_t added = (int64_t) lhs + rhs;
    if (added < 0) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    TruthTableVariablesUInt::assertFits<overflow_error>((uint64_t) added);
    return TruthTableVariablesUInt((uint8_t) added);
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const uint32_t &rhs) {
    return lhs + (uint64_t) rhs;
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const uint64_t &rhs) {
    uint64_t added = (uint8_t) lhs + rhs;
    TruthTableVariablesUInt::assertFits<overflow_error>(added);
    return TruthTableVariablesUInt((uint8_t) added);
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const TruthTableVariablesUInt &rhs) {
    return lhs - (uint64_t) rhs;
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const int32_t &rhs) {
    return lhs - (int64_t) rhs;
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const int64_t &rhs) {
    int64_t subtracted = (int64_t) lhs - rhs;
    if (subtracted < 0) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    TruthTableVariablesUInt::assertFits<overflow_error>((uint64_t) subtracted);
    return TruthTableVariablesUInt((uint8_t) subtracted);
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const uint32_t &rhs) {
    return lhs - (uint64_t) rhs;
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const uint64_t &rhs) {
    if (rhs > lhs) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }

    return TruthTableVariablesUInt((uint8_t) lhs - (uint8_t) rhs);
}

ostream &operator<<(ostream &os, const TruthTableVariablesUInt &val) {
    os << to_string(val);
    return os;
}

istream &operator>>(istream &is, TruthTableVariablesUInt &val) {
    int64_t intVal;
    is >> intVal;
    if (intVal < 0 || intVal > MAX_NUM_VARIABLES) {
        throw out_of_range("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    val = (uint8_t) intVal;
    return is;
}

ostream &operator<<(ostream &os, const __TruthTableValueProxy &val) {
    os << (bool) val;
    return os;
}

static uint64_t getLeftPadding(uint64_t cellWidth, uint64_t itemWidth) {
    uint64_t totalPadding = cellWidth - itemWidth;
    return totalPadding / 2;
}

static uint64_t getRightPadding(uint64_t cellWidth, uint64_t itemWidth) {
    uint64_t totalPadding = cellWidth - itemWidth;
    return totalPadding - getLeftPadding(cellWidth, itemWidth);
}

bool TruthTable::getVariableValueInLine(TruthTableVariablesUInt columnNumber, TruthTableUInt lineIndex) {
    return ((lineIndex >> columnNumber) & (TruthTableUInt) 1) == (TruthTableUInt) 1;
}

ostream &operator<<(ostream &os, const TruthTable &table) {
    uint64_t cellWidth = max_element(table.getVariables().begin(),
                                     table.getVariables().end(),
                                     [](const string &a, const string &b) {
                                        return a.length() < b.length();
                                     })->length() + 2;

    uint64_t totalTableWidth = cellWidth * table.getVariables().size() + // all titles
                               table.getVariables().size() + 1 + // all verticle bars
                               2; // result column

    TruthTableVariablesUInt numVariables = table.getVariables().size();

    // Print the title. Reverse the order so that the LSB is on the right (usual convention)
    {
        for (TruthTableVariablesUInt i = 0; i < numVariables; ++i) {
            const string &var = table.getVariables()[numVariables - i - 1];
            os << '|'
               << string(getLeftPadding(cellWidth, var.length()), ' ')
               << var
               << string(getRightPadding(cellWidth, var.length()), ' ');
        }
        os << '|' << endl;
        os << string(totalTableWidth, '-') << endl;
    }

    // Print the lines
    {
        for (TruthTableUInt i = 0; i < table.size(); ++i) {
            for (TruthTableVariablesUInt j = 0; j < numVariables; ++j) {
                os << '|'
                   << string(getLeftPadding(cellWidth, 1), ' ')
                   << (TruthTable::getVariableValueInLine(numVariables - j - 1, i) ? '1' : '0')
                   << (string(getRightPadding(cellWidth, 1), ' '));
            }
            os << "| " << table[i] << endl;
        }
    }

    return os;
}
}
