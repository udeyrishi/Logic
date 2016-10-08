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
#include <unordered_map>
#include <unordered_set>

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

TruthTable BooleanFunction::combineColumnsWithSameVariables(const TruthTable &table) {
    vector<pair<string, vector<TruthTableVariablesUInt>>> variableIndices = getVariableIndices(table.getVariables());

    bool needsCombining = false;
    for (const auto &variable : variableIndices) {
        if (variable.second.size() > 1) {
            needsCombining = true;
            break;
        }
    }

    if (!needsCombining) {
        return table;
    }

    TruthTableBuilder builder;
    vector<string> variables;
    for (const auto &variable : variableIndices) {
        variables.push_back(variable.first);
    }
    builder.setVariables(variables);

    for (TruthTableUInt i = 0; i < table.size(); ++i) {
        if (shouldKeepLine(variableIndices, i)) {
            builder.set(getLineIndexInCombinedTable((TruthTableVariablesUInt) table.getVariables().size(), i, variableIndices), table[i]);
        }
    }

    return builder.build();
}

TruthTable BooleanFunction::combineTables(BinaryOperator<bool> &_operator, BooleanFunction &other) const {
    // By convention, this function's variables will have lower significance
    vector<string> variables = table.getVariables();
    // TODO: check that the variables don't have the same pointers
    variables.insert(variables.end(), other.getTruthTable().getVariables().begin(), other.getTruthTable().getVariables().end());
    TruthTable resultingTable(variables);

    TruthTableVariablesUInt numVariablesInThis = (TruthTableVariablesUInt) getTruthTable().getVariables().size();
    for (TruthTableUInt i = 0; i < resultingTable.size(); ++i) {
        bool operand1 = getTruthTable()[getFirstTablesIndex(i, numVariablesInThis)];
        bool operand2 = other.getTruthTable()[getOtherTablesIndex(i, numVariablesInThis)];
        resultingTable[i] = _operator(operand1, operand2);
    }

    return resultingTable;
}

BooleanFunction BooleanFunction::operate(UnaryOperator<bool> &_operator) const {
    BooleanFunction result(*this);
    for (TruthTableUInt i = 0; i < table.size(); ++i) {
        result.table[i] = _operator(table[i]);
    }
    return result;
}

BooleanFunction BooleanFunction::operate(BinaryOperator<bool> &_operator, BooleanFunction &other) const {
    return BooleanFunction(combineColumnsWithSameVariables(combineTables(_operator, other)));
}

ostream &operator<<(ostream &os, const BooleanFunction &function) {
    os << function.getTruthTable();
    return os;
}
}