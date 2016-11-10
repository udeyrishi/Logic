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

#include <core/Operators.hpp>
#include <core/Utils.hpp>
#include <unordered_set>
#include <regex>

using namespace std;

namespace Logic {

bool isKnownUnaryOperator(const string &_operator) {
    regex unaryOperators(join<string>({ NOT_REGEX, INDEX_REGEX }, "|"));
    return regex_match(_operator, unaryOperators);
}

bool isKnownBinaryOperator(const string &_operator) {
    regex binaryOperators(join<string>({ AND_REGEX, OR_REGEX, XOR_REGEX, EQUALS_REGEX }, "|"));
    return regex_match(_operator, binaryOperators);
}

UnaryOperator *createUnaryOperatorWithSymbol(const string &_operator) {
    if (regex_match(_operator, regex(NOT_REGEX))) {
        return new Not();
    } else if (regex_match(_operator, regex(INDEX_REGEX))) {
        regex operatorRegex(INDEX_REGEX);
        smatch sm;

        if (!regex_search(_operator, sm, operatorRegex, regex_constants::match_continuous)) {
            throw invalid_argument("Invalid index operator: " + _operator);
        }

        return new Index(stoul(sm[1]));
    }
    throw invalid_argument("Unknown operator: " + _operator);
}

BinaryOperator *createBinaryOperatorWithSymbol(const string &_operator) {
    if (regex_match(_operator, regex(AND_REGEX))) {
        return new And();
    } else if (regex_match(_operator, regex(OR_REGEX)))  {
        return new Or();
    } else if (regex_match(_operator, regex(XOR_REGEX)))  {
        return new Xor();
    } else if (regex_match(_operator, regex(EQUALS_REGEX)))  {
        return new Equals();
    }
    throw invalid_argument("Unknown operator: " + _operator);
}

BooleanFunction BoolTransformationUnaryOperator::operator()(const BooleanFunction &in) const {
    BooleanFunction result(in);
    if (in.isConstant()) {
        result.getConstantValue() = operate(in.getConstantValue());
    } else {
        for (TruthTableUInt i = 0; i < in.getTruthTable().size(); ++i) {
            result.getTruthTable()[i] = operate(in.getTruthTable()[i]);
        }
    }

    return result;
}

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

TruthTable CombinatoryBinaryOperator::combineColumnsWithSameVariables(const TruthTableBuilder &rawBuilder) const {
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

TruthTableBuilder CombinatoryBinaryOperator::combineTables(const BooleanFunction &first, const BooleanFunction &second) const {
    // By convention, this function's variables will have lower significance
    vector<string> variables = first.getTruthTable().getVariables();
    variables.insert(variables.end(), second.getTruthTable().getVariables().begin(), second.getTruthTable().getVariables().end());
    TruthTableBuilder resultingTable;
    resultingTable.setVariables(variables);

    TruthTableVariablesUInt numVariablesInFirst = (TruthTableVariablesUInt) first.getTruthTable().getVariables().size();
    for (TruthTableUInt i = 0; i < resultingTable.tentativeSize(); ++i) {
        bool operand1 = first.getTruthTable()[getFirstTablesIndex(i, numVariablesInFirst)];
        bool operand2 = second.getTruthTable()[getOtherTablesIndex(i, numVariablesInFirst)];
        resultingTable.set(i, operate(operand1, operand2));
    }

    return resultingTable;
}

BooleanFunction CombinatoryBinaryOperator::operator()(const BooleanFunction &first, const BooleanFunction &second) const {
    if (first.hasTruthTable() && second.hasTruthTable()) {
        // Combining two regular Boolean functions
        return BooleanFunction(combineColumnsWithSameVariables(combineTables(first, second)));
    }

    if (!first.hasTruthTable() && !second.hasTruthTable()) {
        // Just combining two constant bools
        return BooleanFunction(operate(first.getConstantValue(), second.getConstantValue()));
    }

    // Combining one truthtable Boolean function with a constant one
    TruthTable clone = first.hasTruthTable() ? first.getTruthTable() : second.getTruthTable();
    for (TruthTableUInt i = 0; i < clone.size(); ++i) {
        // The order of args might be important, because the binary operator may or may not be reflexive
        if (first.hasTruthTable()) {
            clone[i] = operate(first.getTruthTable()[i], second.getConstantValue());
        } else {
            clone[i] = operate(first.getConstantValue(), second.getTruthTable()[i]);
        }
    }
    return BooleanFunction(clone);
}

BooleanFunction Index::operator()(const BooleanFunction &in) const {
    if (in.isConstant()) {
        return BooleanFunction(in.getConstantValue());
    }

    return BooleanFunction(in.getTruthTable()[index]);
}
}
