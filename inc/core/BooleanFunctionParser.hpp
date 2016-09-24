#pragma once

#include <string>
#include <core/Operators.hpp>
#include <core/BooleanFunction.hpp>
#include <stack>

using namespace std;

namespace Logic {
static const string VARIABLE_REGEX("[a-zA-Z]+");

class BooleanFunctionAccumulator {
public:
    void push(const string &variable);
    void push(const UnaryOperator<bool> &_operator);
    void push(const BinaryOperator<bool> &_operator);
    BooleanFunction pop();
private:
    stack<BooleanFunction> _stack;
};

bool isValidVariable(const string &var);

class BooleanFunctionParser {
public:
    /**
     * Parses a string boolean function into a BooleanFunction object.
     * Note: You should always use parenthesis to specify the operator precedence, because there is no agreed upon
     * natural precedence between the different binary operators (unary operators have a natural precedence--they apply to the
     * immediately following operand). For instance: "a | (b & d | !c) & c" can be interpreted as "(a | ((b & d) | !c)) & c"
     * if using "greedy" precedence, or as "a | ((b & (d | !c)) & c)"" if using the "lazy" precedence.

     * The convention for this parser is to use the latter lazy one.
     */
    BooleanFunction parse(const string &function) const;
};
}