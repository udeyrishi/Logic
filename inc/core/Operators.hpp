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
#include <regex>

using namespace std;

namespace Logic {

static const string NOT_REGEX("[!]");
static const string AND_REGEX("[&]");
static const string OR_REGEX("[\\|]");
static const string XOR_REGEX("[\\^]");
static const string EQUALS_REGEX("[=]{2}");
static const vector<string> OPERATOR_REGEXES({
                                          NOT_REGEX,
                                          AND_REGEX,
                                          OR_REGEX,
                                          XOR_REGEX,
                                          EQUALS_REGEX });

template <typename T>
class UnaryOperator {
public:
    virtual T operator()(const T in) = 0;
    virtual ~UnaryOperator() {
    }
};

template <typename T>
class BinaryOperator {
public:
    virtual T operator()(const T first, const T second) = 0;
    virtual ~BinaryOperator() {
    }
};

template <typename T>
class Not : public UnaryOperator<T> {
public:
    virtual T operator()(const T in) {
        return !in;
    }
};

template <typename T>
class Or : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) {
        return first || second;
    }
};

template <typename T>
class And : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) {
        return first && second;
    }
};

template <typename T>
class Xor : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) {
        return first != second;
    }
};

template <typename T>
class Equals : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) {
        return first == second;
    }
};

bool isKnownUnaryOperator(const string &_operator);
bool isKnownBinaryOperator(const string &_operator);

// Need to leave this implementation in the header, because else the linker freaks out
template <typename T>
UnaryOperator<T> *createUnaryOperatorWithSymbol(const string &_operator) {
    if (regex_match(_operator, regex(NOT_REGEX))) {
        return new Not<T>();
    }
    throw invalid_argument("Unknown operator: " + _operator);
}

// Need to leave this implementation in the header, because else the linker freaks out
template <typename T>
BinaryOperator<T> *createBinaryOperatorWithSymbol(const string &_operator) {
    if (regex_match(_operator, regex(AND_REGEX))) {
        return new And<T>();
    } else if (regex_match(_operator, regex(OR_REGEX)))  {
        return new Or<T>();
    } else if (regex_match(_operator, regex(XOR_REGEX)))  {
        return new Xor<T>();
    } else if (regex_match(_operator, regex(EQUALS_REGEX)))  {
        return new Equals<T>();
    }
    throw invalid_argument("Unknown operator: " + _operator);
}
}
