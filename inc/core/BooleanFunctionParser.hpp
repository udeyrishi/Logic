#pragma once

#include <string>
#include <core/Operators.hpp>
#include <core/BooleanFunction.hpp>
#include <stack>
#include <regex>

using namespace std;

namespace Logic {
#define VARIABLE_REGEX "[a-zA-Z]+"

class BooleanFunctionAccumulator {
public:
    void push(const string &variable);
    void push(const UnaryOperator<bool> &_operator);
    void push(const BinaryOperator<bool> &_operator);
    BooleanFunction pop();
private:
    stack<BooleanFunction> _stack;
};

bool isValidVariable(const string &var) {
    return regex_match(var, regex(VARIABLE_REGEX));
}

class BooleanFunctionParser {
public:
    BooleanFunction parse(const string &function) const;
};
}