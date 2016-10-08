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

using namespace std;

namespace Logic {

static const string NOT_SYMBOL("!");
static const string AND_SYMBOL("&");
static const string OR_SYMBOL("|");
static const string XOR_SYMBOL("^");
static const vector<string> OPERATOR_REGEXES({ "["   + NOT_SYMBOL + "]",
                                          "["   + AND_SYMBOL + "]",
                                          "[\\"   + OR_SYMBOL  + "]",
                                          "[\\" + XOR_SYMBOL + "]"});

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

bool isKnownUnaryOperator(const string &_operator);
bool isKnownBinaryOperator(const string &_operator);

// Need to leave this implementation in the header, because else the linker freaks out
template <typename T>
UnaryOperator<T> *createUnaryOperatorWithSymbol(const string &_operator) {
    if (_operator == NOT_SYMBOL) {
        return new Not<T>();
    }
    throw invalid_argument("Unknown operator: " + _operator);
}

// Need to leave this implementation in the header, because else the linker freaks out
template <typename T>
BinaryOperator<T> *createBinaryOperatorWithSymbol(const string &_operator) {
    if (_operator == AND_SYMBOL) {
        return new And<T>();
    } else if (_operator == OR_SYMBOL) {
        return new Or<T>();
    } else if (_operator == XOR_SYMBOL) {
        return new Xor<T>();
    }
    throw invalid_argument("Unknown operator: " + _operator);
}
}
