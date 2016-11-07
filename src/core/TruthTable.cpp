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
#include <core/Exceptions.hpp>
#include <unordered_set>

namespace Logic {
static bool isPowerOfTwo(TruthTableUInt n) {
    return (n > 0) && ((n & (n - 1)) == 0);
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

vector<TruthTableUInt> TruthTable::getMinterms() {
    vector<TruthTableUInt> minterms;
    for (TruthTableUInt i = 0; i < size(); ++i) {
        if ((*this)[i]) {
             minterms.push_back(i);
        }
    }
    return minterms;
}

vector<TruthTableUInt> TruthTable::getMaxterms() {
    vector<TruthTableUInt> maxterms;
    for (TruthTableUInt i = 0; i < size(); ++i) {
        if (!(*this)[i]) {
             maxterms.push_back(i);
        }
    }
    return maxterms;
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
    return TruthTableCondition(this);
}

TruthTableCondition::TruthTableCondition(TruthTable *table) : builder(nullptr), table(table) {
}

TruthTableCondition::TruthTableCondition(const TruthTableCondition &rhs) : builder(nullptr), table(rhs.table) {
    if (rhs.builder != nullptr) {
        this->builder = new TruthTableBuilder(*rhs.builder);
    }
}

TruthTableCondition::~TruthTableCondition() {
    if (builder != nullptr) {
        delete builder;
    }
}

TruthTableCondition &TruthTableCondition::operator=(const TruthTableCondition &rhs) {
    if (this != &rhs) {
        if (builder != nullptr) {
            delete builder;
            builder = nullptr;
        }
        if (rhs.builder != nullptr) {
            this->builder = new TruthTableBuilder(*rhs.builder);
        }

        table = rhs.table;
    }

    return *this;
}

void TruthTableCondition::addCondition(const string &variable, const bool value) {
    const auto &hit = find(table->getVariables().begin(), table->getVariables().end(), variable);
    if (hit == table->getVariables().end()) {
        throw invalid_argument("variable not found in the truth table: " + variable);
    }

    TruthTableVariablesUInt position = hit - table->getVariables().begin();
    if (conditions.find(position) != conditions.end()) {
        conditions.erase(position);
    }

    conditions.insert(make_pair(position, value));
}

void TruthTableCondition::process() {
    // reset results from last process() call, if any
    if (builder != nullptr) {
        delete builder;
        builder = nullptr;
    }

    TruthTableUInt newTable = 0;
    builder = new TruthTableBuilder();
    for (TruthTableUInt i = 0; i < table->size(); ++i) {
        bool allConditionsMet = true;
        for (const auto &condition : conditions) {
            if ((bool) ((i >> condition.first) & 1) != condition.second) {
                allConditionsMet = false;
                break;
            }
        }

        if (allConditionsMet) {
            builder->set(newTable++, (*table)[i]);
        }
    }

    vector<string> newVariables;
    for (TruthTableVariablesUInt i = 0; i < table->getVariables().size(); ++i) {
        if (conditions.find(i) == conditions.end()) {
            newVariables.push_back(table->getVariables()[i]);
        }
    }
    builder->setVariables(newVariables);
}

bool TruthTableCondition::hasCollapsedToConstant() const {
    if (builder == nullptr) {
        throw IllegalStateException("Cannot assess the TruthTableCondition's state before a process() call.");
    }

    return builder->tentativeSize() == 1;
}

bool TruthTableCondition::getConstant() const {
    if (hasCollapsedToConstant()) {
        return builder->getValue(0);
    }

    throw IllegalStateException("The result after applying the conditions is not a constant value.");
}

TruthTable TruthTableCondition::getTruthTable() const {
    if (hasCollapsedToConstant()) {
        throw IllegalStateException("The result after applying the conditions is a constant value.");
    }

    return builder->build();
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
            os << "| " << table[i];
            if (i < table.size() - 1) {
                os << endl;
            }
        }
    }

    return os;
}
}
