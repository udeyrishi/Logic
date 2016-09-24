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
    virtual T operator()(const T in) const = 0;
    virtual ~UnaryOperator() {
    }
};

template <typename T>
class BinaryOperator {
public:
    virtual T operator()(const T first, const T second) const = 0;
    virtual ~BinaryOperator() {
    }
};

template <typename T>
class Not : public UnaryOperator<T> {
public:
    virtual T operator()(const T in) const {
        return !in;
    }
};

template <typename T>
class Or : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) const {
        return first || second;
    }
};

template <typename T>
class And : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) const {
        return first && second;
    }
};

template <typename T>
class Xor : public BinaryOperator<T> {
public:
    virtual T operator()(const T first, const T second) const {
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
