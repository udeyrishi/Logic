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

#include <stdexcept>
#include <string>
#include <vector>
#include <core/BooleanFunction.hpp>
#include <core/TruthTable.hpp>

using namespace std;

namespace Logic {

static const string NOT_REGEX("[!]");
static const string AND_REGEX("[&]");
static const string OR_REGEX("[\\|]");
static const string XOR_REGEX("[\\^]");
static const string EQUALS_REGEX("[=]{2}");
// The CONDITIONS_REGEX was match the INDEX_REGEX strings as well, so
// check for the INDEX_REGEX first
static const string INDEX_REGEX("[\\[]{1}[\\s]*([\\d]+)[\\s]*[\\]]{1}");
static const string CONDITIONS_REGEX("[\\[]{1}[\\s]*(.+)[\\s]*[\\]]{1}");
static const vector<string> OPERATOR_REGEXES({
                                          NOT_REGEX,
                                          AND_REGEX,
                                          OR_REGEX,
                                          XOR_REGEX,
                                          EQUALS_REGEX,
                                          INDEX_REGEX,
                                          CONDITIONS_REGEX });

class UnaryOperator {
public:
    virtual BooleanFunction operator()(const BooleanFunction &in) const = 0;
    virtual ~UnaryOperator() {
    }
};

class BinaryOperator {
public:
    virtual BooleanFunction operator()(const BooleanFunction &first, const BooleanFunction &second) const = 0;
    virtual ~BinaryOperator() {
    }
};

class BoolTransformationUnaryOperator : public UnaryOperator {
public:
    virtual BooleanFunction operator()(const BooleanFunction &in) const;

private:
    virtual bool operate(const bool in) const = 0;
};

class Not : public BoolTransformationUnaryOperator {
private:
    virtual bool operate(const bool in) const {
        return !in;
    }
};

class Equals : public BinaryOperator {
public:
    virtual BooleanFunction operator()(const BooleanFunction &first, const BooleanFunction &second) const {
        return first == second;
    }
};

class CombinatoryBinaryOperator : public BinaryOperator {
public:
    virtual BooleanFunction operator()(const BooleanFunction &first, const BooleanFunction &second) const;

private:
    TruthTable combineColumnsWithSameVariables(const TruthTableBuilder &rawBuilder) const;
    TruthTableBuilder combineTables(const BooleanFunction &first, const BooleanFunction &second) const;
    virtual bool operate(const bool first, const bool second) const = 0;
};

class Or : public CombinatoryBinaryOperator {
private:
    virtual bool operate(const bool first, const bool second) const {
        return first || second;
    }
};

class And : public CombinatoryBinaryOperator {
private:
    virtual bool operate(const bool first, const bool second) const {
        return first && second;
    }
};

class Xor : public CombinatoryBinaryOperator {
private:
    virtual bool operate(const bool first, const bool second) const {
        return first != second;
    }
};

class Index : public UnaryOperator {
public:
    Index(const TruthTableUInt index) : index(index) {
    }

    virtual BooleanFunction operator()(const BooleanFunction &in) const;
private:
    const TruthTableUInt index;
};

class Conditions : public UnaryOperator {
public:
    Conditions(const vector<pair<string, bool>> &conditions) : conditions(conditions) {
    }

    virtual BooleanFunction operator()(const BooleanFunction &in) const;

private:
    const vector<pair<string, bool>> conditions;
};

bool isKnownPrefixUnaryOperator(const string &_operator);
bool isKnownSuffixUnaryOperator(const string &_operator);
bool isKnownUnaryOperator(const string &_operator);
bool isKnownBinaryOperator(const string &_operator);

UnaryOperator *createUnaryOperatorWithSymbol(const string &_operator);
BinaryOperator *createBinaryOperatorWithSymbol(const string &_operator);
}
