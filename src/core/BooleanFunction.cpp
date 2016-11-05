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

#include <core/BooleanFunction.hpp>
#include <core/Exceptions.hpp>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;

namespace Logic {
static TruthTableUInt getOtherTablesIndex(const TruthTableUInt combinedTablesIndex, const TruthTableVariablesUInt numVariablesInFirstTable) {
    TruthTableUInt mask = ~((((TruthTableUInt) 1) << numVariablesInFirstTable) - 1);
    return (combinedTablesIndex & mask) >> numVariablesInFirstTable;
}

static TruthTableUInt getFirstTablesIndex(const TruthTableUInt combinedTablesIndex, const TruthTableVariablesUInt numVariablesInFirstTable) {
    TruthTableUInt mask = (((TruthTableUInt) 1) << numVariablesInFirstTable) - 1;
    return combinedTablesIndex & mask;
}

static vector<pair<string, vector<TruthTableVariablesUInt>>> getVariableIndices(const vector<string> &variables) {
    vector<pair<string, vector<TruthTableVariablesUInt>>> result;
    unordered_map<string, TruthTableVariablesUInt> variablesSeen;

    for (TruthTableVariablesUInt i = 0; i < variables.size(); ++i) {
        const string &variable = variables[i];
        if (variablesSeen.find(variable) == variablesSeen.end()) {
            result.push_back(pair<string, vector<TruthTableVariablesUInt>>(variable, {}));
            variablesSeen[variable] = (TruthTableVariablesUInt) (result.size() - 1);
        }

        result[variablesSeen[variable]].second.push_back(i);
    }

    return result;
}

static TruthTableUInt getLineIndexInCombinedTable(const TruthTableVariablesUInt numVariablesInOriginalTable,
                                     const TruthTableUInt originalTableLineIndex,
                                     const vector<pair<string, vector<TruthTableVariablesUInt>>> &variableIndices) {
    unordered_set<TruthTableVariablesUInt> columnsToRemove;
    for (const auto &variable : variableIndices) {
        // Keep the 0th, remove all others
        for (TruthTableVariablesUInt i = 1; i < variable.second.size(); ++i) {
            columnsToRemove.insert(variable.second[i]);
        }
    }

    TruthTableUInt wordForNewLine = 0;
    TruthTableVariablesUInt newLineColumn = 0;
    for (TruthTableVariablesUInt column = 0; column < numVariablesInOriginalTable; ++column) {
        if (columnsToRemove.find(column) == columnsToRemove.end()) {
            wordForNewLine += (TruthTable::getVariableValueInLine(column, originalTableLineIndex) ? (TruthTableUInt)1 : (TruthTableUInt)0) << newLineColumn++;
        }
    }

    return wordForNewLine;
}

static bool shouldKeepLine(const vector<pair<string, vector<TruthTableVariablesUInt>>> &variableIndices, TruthTableUInt lineIndex) {
    for (const auto &variable : variableIndices) {
        bool previous = TruthTable::getVariableValueInLine(variable.second[0], lineIndex);
        for (TruthTableUInt i = 1; i < variable.second.size(); ++i) {
            if (previous != TruthTable::getVariableValueInLine(variable.second[i], lineIndex)) {
                return false;
            }
        }
    }
    return true;
}

TruthTable BooleanFunction::combineColumnsWithSameVariables(const TruthTableBuilder &rawBuilder) {
    vector<pair<string, vector<TruthTableVariablesUInt>>> variableIndices = getVariableIndices(rawBuilder.getVariables());

    bool needsCombining = false;
    for (const auto &variable : variableIndices) {
        if (variable.second.size() > 1) {
            needsCombining = true;
            break;
        }
    }

    if (!needsCombining) {
        return rawBuilder.build();
    }

    TruthTableBuilder builder;
    vector<string> variables;
    for (const auto &variable : variableIndices) {
        variables.push_back(variable.first);
    }
    builder.setVariables(variables);

    for (TruthTableUInt i = 0; i < rawBuilder.tentativeSize(); ++i) {
        if (shouldKeepLine(variableIndices, i)) {
            builder.set(getLineIndexInCombinedTable((TruthTableVariablesUInt) rawBuilder.getVariables().size(), i, variableIndices), rawBuilder.getValue(i));
        }
    }

    return builder.build();
}

TruthTableBuilder BooleanFunction::combineTables(BinaryOperator<bool> &_operator, BooleanFunction &other) const {
    // By convention, this function's variables will have lower significance
    vector<string> variables = table->getVariables();
    variables.insert(variables.end(), other.getTruthTable().getVariables().begin(), other.getTruthTable().getVariables().end());
    TruthTableBuilder resultingTable;
    resultingTable.setVariables(variables);

    TruthTableVariablesUInt numVariablesInThis = (TruthTableVariablesUInt) getTruthTable().getVariables().size();
    for (TruthTableUInt i = 0; i < resultingTable.tentativeSize(); ++i) {
        bool operand1 = getTruthTable()[getFirstTablesIndex(i, numVariablesInThis)];
        bool operand2 = other.getTruthTable()[getOtherTablesIndex(i, numVariablesInThis)];
        resultingTable.set(i, _operator(operand1, operand2));
    }

    return resultingTable;
}

BooleanFunction BooleanFunction::operate(UnaryOperator<bool> &_operator) const {
    BooleanFunction result(*this);
    if (isConstant()) {
        result.getConstantValue() = _operator(getConstantValue());
    } else {
        for (TruthTableUInt i = 0; i < table->size(); ++i) {
            result.getTruthTable()[i] = _operator(getTruthTable()[i]);
        }
    }

    return result;
}

BooleanFunction BooleanFunction::operate(BinaryOperator<bool> &_operator, BooleanFunction &other) const {
    if (hasTruthTable() && other.hasTruthTable()) {
        // Combining two regular Boolean functions
        return BooleanFunction(combineColumnsWithSameVariables(combineTables(_operator, other)));
    }

    if (!hasTruthTable() && !other.hasTruthTable()) {
        // Just combining two constant bools
        return BooleanFunction(_operator(getConstantValue(), other.getConstantValue()));
    }

    // Combining one truthtable Boolean function with a constant one
    TruthTable clone = hasTruthTable() ? getTruthTable() : other.getTruthTable();
    for (TruthTableUInt i = 0; i < clone.size(); ++i) {
        // The order of args might be important, because the binary operator may or may not be reflexive
        if (hasTruthTable()) {
            clone[i] = _operator(getTruthTable()[i], other.getConstantValue());
        } else {
            clone[i] = _operator(getConstantValue(), other.getTruthTable()[i]);
        }
    }
    return BooleanFunction(clone);
}

ostream &operator<<(ostream &os, const BooleanFunction &function) {
    if (function.isConstant()) {
        os << (function.getConstantValue() ? 1 : 0);
    } else {
        os << function.getTruthTable();
    }

    return os;
}

bool operator==(const BooleanFunction &left, const BooleanFunction &right) {
    if (left.isConstant() && right.isConstant()) {
        return left.getConstantValue() == right.getConstantValue();
    }

    if (left.hasTruthTable() && right.hasTruthTable()) {
        return left.getTruthTable() == right.getTruthTable();
    }

    return false;
}

BooleanFunction::BooleanFunction(const TruthTable &table) : constValue(nullptr) {
    this->table = new TruthTable(table);
}

BooleanFunction::BooleanFunction(const bool constValue) : table(nullptr) {
    this->constValue = new bool(constValue);
}

BooleanFunction::BooleanFunction(const BooleanFunction &rhs) {
    init(rhs);
}

BooleanFunction &BooleanFunction::operator=(const BooleanFunction &rhs) {
    if (this != &rhs) {
        destroy();
        init(rhs);
    }
    return *this;
}

BooleanFunction::~BooleanFunction() {
    destroy();
}

void BooleanFunction::init(const BooleanFunction &rhs) {
    this->constValue = rhs.constValue == nullptr ? nullptr : new bool(*rhs.constValue);
    this->table = rhs.table == nullptr ? nullptr : new TruthTable(*rhs.table);
}

void BooleanFunction::destroy() {
    if (constValue != nullptr) {
        delete constValue;
        constValue = nullptr;
    }

    if (table != nullptr) {
        delete table;
        table = nullptr;
    }
}

bool BooleanFunction::hasTruthTable() const {
    return table != nullptr;
}

TruthTable &BooleanFunction::getTruthTable() {
    if (hasTruthTable()) {
        return *table;
    }

    throw IllegalStateException("Cannot get the truth table of a constant value Boolean function.");
}

const TruthTable &BooleanFunction::getTruthTable() const {
    if (hasTruthTable()) {
        return *table;
    }

    throw IllegalStateException("Cannot get the truth table of a constant value Boolean function.");
}

bool BooleanFunction::isConstant() const {
    return constValue != nullptr;
}

bool BooleanFunction::getConstantValue() const {
    if (isConstant()) {
        return *constValue;
    }

    throw IllegalStateException("Cannot get the constant value of a non-constant Boolean function.");
}

bool &BooleanFunction::getConstantValue() {
    if (isConstant()) {
        return *constValue;
    }

    throw IllegalStateException("Cannot get the constant value of a non-constant Boolean function.");
}
}
