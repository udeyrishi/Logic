#include <core/BooleanFunction.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace Logic;
using namespace std;

uint64_t getOtherTablesIndex(const uint64_t combinedTablesIndex, const uint32_t numVariablesInFirstTable) {
    uint64_t mask = ~(uint64_t)((((uint32_t)1) << numVariablesInFirstTable) - 1);
    return (combinedTablesIndex & mask) >> numVariablesInFirstTable;
}

uint64_t getFirstTablesIndex(const uint64_t combinedTablesIndex, const uint32_t numVariablesInFirstTable) {
    uint64_t mask = (uint64_t)((((uint32_t)1) << numVariablesInFirstTable) - 1);
    return combinedTablesIndex & mask;
}

vector<pair<string, vector<uint32_t>>> getVariableIndices(const vector<string> &variables) {
    vector<pair<string, vector<uint32_t>>> result;
    unordered_map<string, uint32_t> variablesSeen;

    for (uint32_t i = 0; i < variables.size(); ++i) {
        const string &variable = variables[i];
        if (variablesSeen.find(variable) == variablesSeen.end()) {
            result.push_back(pair<string, vector<uint32_t>>(variable, {}));
            variablesSeen[variable] = (uint32_t) result.size() - 1;
        }

        result[variablesSeen[variable]].second.push_back(i);
    }

    return result;
}

bool getVariableValueInLine(uint32_t columnNumber, uint64_t lineIndex) {
    return ((lineIndex >> columnNumber) & 1) == 1;
}

uint64_t getLineIndexInCombinedTable(const uint32_t numVariablesInOriginalTable,
                                     const uint64_t originalTableLineIndex,
                                     const vector<pair<string, vector<uint32_t>>> &variableIndices) {
    unordered_set<uint32_t> columnsToRemove;
    for (const auto &variable : variableIndices) {
        // Keep the 0th, remove all others
        for (uint32_t i = 1; i < variable.second.size(); ++i) {
            columnsToRemove.insert(variable.second[i]);
        }
    }

    uint64_t wordForNewLine = 0;
    uint32_t newLineColumn = 0;
    for (uint32_t column = 0; column < numVariablesInOriginalTable; ++column) {
        if (columnsToRemove.find(column) == columnsToRemove.end()) {
            wordForNewLine += (getVariableValueInLine(column, originalTableLineIndex) ? (uint64_t)1 : (uint64_t)0) << newLineColumn++;
        }
    }

    return wordForNewLine;
}

bool shouldKeepLine(const vector<pair<string, vector<uint32_t>>> &variableIndices, uint64_t lineIndex) {
    for (const auto &variable : variableIndices) {
        bool previous = getVariableValueInLine(variable.second[0], lineIndex);
        for (uint64_t i = 1; i < variable.second.size(); ++i) {
            if (previous != getVariableValueInLine(variable.second[i], lineIndex)) {
                return false;
            }
        }
    }
    return true;
}

TruthTable BooleanFunction::combineColumnsWithSameVariables(const TruthTable &table) {
    vector<pair<string, vector<uint32_t>>> variableIndices = getVariableIndices(table.getVariables());

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

    for (uint64_t i = 0; i < table.size(); ++i) {
        if (shouldKeepLine(variableIndices, i)) {
            builder.set(getLineIndexInCombinedTable((uint32_t) table.getVariables().size(), i, variableIndices), table[i]);
        }
    }

    return builder.build();
}

TruthTable BooleanFunction::combineTables(const BinaryOperator<bool> &_operator, BooleanFunction &other) const {
    // By convention, this function's variables will have lower significance
    vector<string> variables = table.getVariables();
    // TODO: check that the variables don't have the same pointers
    variables.insert(variables.end(), other.getTruthTable().getVariables().begin(), other.getTruthTable().getVariables().end());
    TruthTable resultingTable(variables);

    uint32_t numVariablesInThis = (uint32_t) getTruthTable().getVariables().size();
    for (uint64_t i = 0; i < resultingTable.size(); ++i) {
        bool operand1 = getTruthTable()[getFirstTablesIndex(i, numVariablesInThis)];
        bool operand2 = other.getTruthTable()[getOtherTablesIndex(i, numVariablesInThis)];
        resultingTable[i] = _operator(operand1, operand2);
    }

    return TruthTable({});
}

BooleanFunction BooleanFunction::operate(const UnaryOperator<bool> &_operator) const {
    BooleanFunction result(*this);
    for (uint64_t i = 0; i < table.size(); ++i) {
        result.table[i] = _operator(table[i]);
    }
    return result;
}

BooleanFunction BooleanFunction::operate(const BinaryOperator<bool> &_operator, BooleanFunction &other) const {
    return BooleanFunction(combineColumnsWithSameVariables(combineTables(_operator, other)));
}