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

#include <core/BooleanFunctionParser.hpp>
#include <vector>
#include <stdint.h>
#include <mutex>
#include <regex>
#include <core/Exceptions.hpp>
#include <core/Utils.hpp>
#include <utility>

using namespace Logic;
using namespace std;

static mutex infixTokenRegexMutex;

static string join(const vector<string> &vec, const string &delimiter) {
    string joined;
    for (uint64_t i = 0; i < vec.size(); ++i) {
        if (i == vec.size() - 1) {
            joined += vec[i];
        } else {
            joined += vec[i] + delimiter;
        }
    }
    return joined;
}

template <typename T>
static T topAndPop(stack<T> &_stack) {
    T top = _stack.top();
    _stack.pop();
    return top;
}

namespace Logic {
static const string VARIABLE_REGEX = "[\\$a-zA-Z]+";
static string getInfixTokensRegex() {
    static string result;
    unique_lock<mutex> lock(infixTokenRegexMutex);
    if (result.length() == 0) {
        vector<string> tokens = OPERATOR_REGEXES;
        tokens.push_back("[\\(]");
        tokens.push_back("[\\)]");
        tokens.push_back(VARIABLE_REGEX);
        tokens.push_back("[1]");
        tokens.push_back("[0]");
        result = "[\\s]*(" + // Optional leading spaces
                 join(tokens, "|") + // actual token regex
                 ")[\\s]*"; // optional trailing spaces
    }
    return result;
}

void BooleanFunctionAccumulator::push(const BooleanFunction &function) {
    _stack.push(function);
}

void BooleanFunctionAccumulator::push(UnaryOperator<bool> &_operator) {
    if (_stack.empty()) {
        throw IllegalStateException("Cannot push a unary operator on an empty stack.");
    }
    _stack.push(topAndPop(_stack).operate(_operator));
}

void BooleanFunctionAccumulator::push(BinaryOperator<bool> &_operator) {
    if (_stack.size() < 2) {
        throw IllegalStateException("Cannot push a binary operator on an stack of size less than 2");
    }
    BooleanFunction operand2 = topAndPop(_stack);
    BooleanFunction operand1 = topAndPop(_stack);
    _stack.push(operand1.operate(_operator, operand2));
}

BooleanFunction BooleanFunctionAccumulator::pop() {
    if (canBePopped()) {
        return topAndPop(_stack);
    }

    throw IllegalStateException("Cannot finish accumulation, because the stack size is still > 1");
}

bool BooleanFunctionAccumulator::canBePopped() {
    return _stack.size() == 1;
}

// This will wrap even single variable names for easier application of unary operators
static vector<string> getInfixTokens(const string &function) {
    regex tokensRegex(getInfixTokensRegex());
    smatch sm;
    vector<string> tokens;
    uint64_t i = 0;
    // regex_search can't take an r-value. Need to store it in a temp variable
    string substring;
    while (i < function.length() && regex_search((substring = function.substr(i, string::npos)), sm, tokensRegex, regex_constants::match_continuous)) {
        // Index 1 => The first group in the regex
        string token = sm[1];
        if (regex_match(token, regex(VARIABLE_REGEX))) {
            // Surround by parenthesis, so that the unary operators are properly applied
            tokens.push_back("(");
            tokens.push_back(token);
            tokens.push_back(")");
        } else {
            tokens.push_back(token);
        }
        i += (uint64_t) sm.position(1) + (uint64_t) sm[1].length();
    }
    if (i != function.length()) {
        // prematurely failed lookup because of match_continuous flag. Unknown token at index i
        throw UnknownTokenException("Unknown token in the string at index " + to_string(i) + " in the boolean function");
    }
    return tokens;
}

static vector<string> getPostfixTokens(const string &function) {
    // Variables have parenthesis around them always, so that unary operators can be properly applied
    vector<string> infixTokens = getInfixTokens(function);
    stack<string> operatorStack;
    vector<string> postfixTokens;

    // These are the regexes of the tokens that are allowed before a ")".
    // In other words, these can be put in the operator stack until a ")" is encountered
    vector<string> operatorStackItems = OPERATOR_REGEXES;
    operatorStackItems.push_back("[\\(]");
    const regex stackItemsRegex(join(operatorStackItems, "|"));
    smatch sm;

    for (const string &token : infixTokens) {
        if (token == ")") {
            while (true) {
                if (operatorStack.empty()) {
                    throw BadBooleanFunctionException("Unbalanced parenthesis: " + function);
                }

                string top = topAndPop(operatorStack);
                if (top == "(") {
                    // unary operators have the opposite precedence. They need to applied to first
                    while (!operatorStack.empty() && isKnownUnaryOperator(operatorStack.top())) {
                        string unaryOperator = topAndPop(operatorStack);
                        postfixTokens.push_back(unaryOperator);
                    }
                    break;
                }
                postfixTokens.push_back(top);
            }
        } else if (regex_match(token, sm, stackItemsRegex)) {
            operatorStack.push(token);
        } else {
            // Not ")" or "(", and not matching operator regexes, but still in the list of infix tokens
            // => must be a variable name or a constant
            postfixTokens.push_back(token);
        }
    }

    while (!operatorStack.empty()) {
        string top = topAndPop(operatorStack);
        if (top == "(") {
            throw BadBooleanFunctionException("Unbalanced parenthesis: " + function);
        }
        postfixTokens.push_back(top);
    }

    return postfixTokens;
}

BooleanFunction BooleanFunctionParser::parse(const string &function) const {
    return parse(function, [](const string &functionName) -> const BooleanFunction& {
        throw BooleanFunctionNotFoundException("Boolean function not found: " + functionName);
    });
}

static string CONDITION_SPECIFIER = ":";

vector<pair<string, bool>> parseConditions(const string &rawConditions) {
    const vector<string> conditionStrings = split(rawConditions, ',');
    if (conditionStrings.empty()) {
        throw BadBooleanFunctionException("No conditions specified after the operator '" + CONDITION_SPECIFIER + "'");
    }

    vector<pair<string, bool>> conditions;
    for (const string &condition : conditionStrings) {
        vector<string> conditionDef = split(trim(condition), '=');
        if (conditionDef.size() != 2) {
            throw BadBooleanFunctionException("Illegal condition format: " + condition);
        }

        string var = trim(conditionDef[0]);
        string val = trim(conditionDef[1]);
        if (var.empty() || val.empty()) {
            throw BadBooleanFunctionException("Illegal condition format: " + condition);
        }

        bool boolVal;
        if (val == "1") {
            boolVal = true;
        } else if (val == "0") {
            boolVal = false;
        } else {
            throw BadBooleanFunctionException("Illegal condition value " + val + " for variable " + var);
        }
        conditions.push_back(make_pair(var, boolVal));
    }
    return conditions;
}

BooleanFunction BooleanFunctionParser::parse(const string &function, std::function<const BooleanFunction& (const string&)> lookupFunction) const {
    size_t indexOfConditionSpecifier = function.find(CONDITION_SPECIFIER);
    vector<string> postfixTokens = getPostfixTokens(trim(indexOfConditionSpecifier == string::npos ? function : function.substr(0, indexOfConditionSpecifier)));
    BooleanFunctionAccumulator accumulator;
    for (const string &token : postfixTokens) {
        if (isKnownUnaryOperator(token)) {
            UnaryOperator<bool> *op = createUnaryOperatorWithSymbol<bool>(token);
            accumulator.push(*op);
            delete op;
        } else if (isKnownBinaryOperator(token)) {
            BinaryOperator<bool> *op = createBinaryOperatorWithSymbol<bool>(token);
            accumulator.push(*op);
            delete op;
        } else {
            // token == variable
            if (token.c_str()[0] == '$') {
                string lookupFunctionName = token.substr(1, string::npos);
                if (lookupFunctionName.length() == 0) {
                    // someone used $ as a variable name
                    throw UnknownTokenException("'$' is reserved token, and cannot be used as a variable name.");
                }
                const BooleanFunction &function = lookupFunction(lookupFunctionName);
                accumulator.push(function);
            } else {
                if (token == "0" || token == "1") {
                    accumulator.push(BooleanFunction(token == "1"));
                } else {
                    BooleanFunction function(TruthTable({ token }));
                    function.getTruthTable()[0] = false;
                    function.getTruthTable()[1] = true;
                    accumulator.push(function);
                }
            }
        }
    }

    if (!accumulator.canBePopped()) {
        throw BadBooleanFunctionException("Missing operator tokens in the boolean function.");
    }

    BooleanFunction parsed = accumulator.pop();
    if (indexOfConditionSpecifier == string::npos) {
        return parsed;
    }

    const vector<pair<string, bool>> conditions = parseConditions(trim(function.substr(indexOfConditionSpecifier + 1, string::npos)));
    TruthTableCondition truthTableCondition = parsed.getTruthTable().conditionBuilder();
    for (const pair<string, bool> &condition : conditions) {
        truthTableCondition.addCondition(condition.first, condition.second);
    }
    truthTableCondition.process();

    if (truthTableCondition.hasCollapsedToConstant()) {
        return BooleanFunction(truthTableCondition.getConstant());
    } else {
        return BooleanFunction(truthTableCondition.getTruthTable());
    }
}
}
