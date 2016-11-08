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

#include <vector>
#include <string>
#include <cmath>
#include <core/TruthTableTypes.hpp>

using namespace std;

namespace Logic {

class __TruthTableValueProxy;
class TruthTableCondition;

class TruthTable {
public:
    TruthTable(const vector<string> &variables);

    const vector<string> &getVariables() const {
        return variables;
    }

    TruthTableUInt size() const {
        return (TruthTableUInt) pow(2, variables.size());
    }

    __TruthTableValueProxy operator[](const TruthTableUInt index);
    bool operator[](const TruthTableUInt index) const;

    TruthTableCondition conditionBuilder();

    vector<TruthTableUInt> getMinterms();
    vector<TruthTableUInt> getMaxterms();

    static bool getVariableValueInLine(TruthTableVariablesUInt columnNumber, TruthTableUInt lineIndex);
private:
    vector<string> variables;
    vector<bool> values;

    void validateIndex(const TruthTableUInt index) const;

    friend class __TruthTableValueProxy;
    friend bool operator==(const TruthTable &left, const TruthTable &right);
};

ostream &operator<<(ostream &os, const TruthTable &table);
bool operator==(const TruthTable &left, const TruthTable &right);

// This type should not be used directly by keeping a variable. It should be either converted to a bool, or used for assignment.
class __TruthTableValueProxy {
public:
    __TruthTableValueProxy(const TruthTableUInt index, TruthTable &table) : index(index), table(table) {
    }

    operator bool() const {
        return table.values[index];
    }

    void operator=(const bool value) {
        table.values[index] = value;
    }

private:
    const TruthTableUInt index;
    TruthTable &table;
};

ostream &operator<<(ostream &os, const __TruthTableValueProxy &val);

class TruthTableBuilder {
public:
    void set(TruthTableUInt lineIndex, const bool b);

    void setVariables(const vector<string> &variables) {
        this->variables = variables;
    }

    vector<string> &getVariables() {
        return variables;
    }

    vector<string> getVariables() const {
        return variables;
    }

    bool getValue(const TruthTableUInt i) const {
        return values[i];
    }

    TruthTableUInt tentativeSize() const;

    TruthTable build() const;

private:
    vector<bool> values;
    vector<string> variables;
};

class TruthTableCondition {
public:
    TruthTableCondition(const TruthTableCondition &rhs);
    ~TruthTableCondition();
    TruthTableCondition &operator=(const TruthTableCondition &rhs);

    void addCondition(const string &variable, const bool value);
    void process();
    bool hasCollapsedToConstant() const;
    bool getConstant() const;
    TruthTable getTruthTable() const;
private:
    TruthTableCondition(TruthTable *table);

    friend class TruthTable;

    TruthTableBuilder *builder;
    // TruthTableCondition does NOT own table
    TruthTable *table;
    unordered_map<TruthTableVariablesUInt, bool> conditions;
};
}
