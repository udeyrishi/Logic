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

template <typename T>
static T topAndPop(stack<T> &_stack) {
    T top = _stack.top();
    _stack.pop();
    return top;
}

namespace Logic {
static const string VARIABLE_REGEX_WITH_REF = "[\\$]?" + VARIABLE_REGEX;

static string getInfixTokensRegex() {
    static string result;
    unique_lock<mutex> lock(infixTokenRegexMutex);
    if (result.length() == 0) {
        vector<string> tokens = OPERATOR_REGEXES;
        tokens.push_back("[\\(]");
        tokens.push_back("[\\)]");
        tokens.push_back(VARIABLE_REGEX_WITH_REF);
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

void BooleanFunctionAccumulator::push(UnaryOperator &_operator) {
    if (_stack.empty()) {
        throw IllegalStateException("Cannot push a unary operator on an empty stack.");
    }
    _stack.push(_operator(topAndPop(_stack)));
}

void BooleanFunctionAccumulator::push(BinaryOperator &_operator) {
    if (_stack.size() < 2) {
        throw IllegalStateException("Cannot push a binary operator on an stack of size less than 2");
    }
    BooleanFunction operand2 = topAndPop(_stack);
    BooleanFunction operand1 = topAndPop(_stack);
    _stack.push(_operator(operand1, operand2));
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
        if (regex_match(token, regex(VARIABLE_REGEX_WITH_REF))) {
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

    for (size_t i = 0; i < infixTokens.size(); ++i) {
        const string &token = infixTokens[i];

        // Validate that if the token is a prefix unary operator, it has a valid token that follows
        if (isKnownPrefixUnaryOperator(token)) {
            // prefix unary operator can't be the last token
            if (i == infixTokens.size() - 1 ||
            // prefix unary operator can only follow a parenthesis or another prefix unary operator
                !(infixTokens[i+1] == "(" || isKnownPrefixUnaryOperator(infixTokens[i+1]))) {
                throw BadBooleanFunctionException("Misplaced prefix unary operator: " + token);
            }
        }

        // Validate that if the token is a suffix unary operator, it has a valid token that followed
        if (isKnownSuffixUnaryOperator(token)) {
            // suffix unary operator can't be the first token
            if (i == 0 ||
                !(infixTokens[i-1] == ")" || isKnownSuffixUnaryOperator(infixTokens[i-1]))) {
                throw BadBooleanFunctionException("Misplaced suffix unary operator: " + token);
            }
        }

        // At this point, we know that prefix and suffix unary operators are at the correct logical locations

        if (token == ")") {
            while (true) {
                if (operatorStack.empty()) {
                    throw BadBooleanFunctionException("Unbalanced parenthesis: " + function);
                }

                string top = topAndPop(operatorStack);
                if (top == "(") {
                    // Prefix unary operators are expected right before the opening parenthesis.
                    // So push these to the postFixTokens list right here
                    while (!operatorStack.empty() && isKnownPrefixUnaryOperator(operatorStack.top())) {
                        string unaryOperator = topAndPop(operatorStack);
                        postfixTokens.push_back(unaryOperator);
                    }
                    break;
                } else {
                    postfixTokens.push_back(top);
                }
            }
        } else if (regex_match(token, sm, stackItemsRegex)) {
            if (isKnownSuffixUnaryOperator(token)) {
                postfixTokens.push_back(token);
            } else {
                operatorStack.push(token);
            }
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

BooleanFunction BooleanFunctionParser::parse(const string &function, std::function<const BooleanFunction& (const string&)> lookupFunction) const {
    vector<string> postfixTokens = getPostfixTokens(trim(function));
    BooleanFunctionAccumulator accumulator;
    for (const string &token : postfixTokens) {
        if (isKnownUnaryOperator(token)) {
            UnaryOperator *op;
            try {
                op = createUnaryOperatorWithSymbol(token);
            } catch (const invalid_argument &ex) {
                throw BadBooleanFunctionException(ex.what());
            }
            accumulator.push(*op);
            delete op;
        } else if (isKnownBinaryOperator(token)) {
            BinaryOperator *op;
            try {
                op = createBinaryOperatorWithSymbol(token);
            } catch (const invalid_argument &ex) {
                throw BadBooleanFunctionException(ex.what());
            }
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

    return accumulator.pop();
}
}
