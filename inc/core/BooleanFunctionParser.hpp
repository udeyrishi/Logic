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

#include <string>
#include <core/Operators.hpp>
#include <core/BooleanFunction.hpp>
#include <stack>
#include <functional>

using namespace std;

namespace Logic {
class BooleanFunctionAccumulator {
public:
    void push(const BooleanFunction &function);
    void push(UnaryOperator<bool> &_operator);
    void push(BinaryOperator<bool> &_operator);
    BooleanFunction pop();
    bool canBePopped();
private:
    stack<BooleanFunction> _stack;
};

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

    // The lookup function will be used for resolving variables stating with '$'
    BooleanFunction parse(const string &function, std::function<const BooleanFunction& (const string&)> lookupFunction) const;
};
}