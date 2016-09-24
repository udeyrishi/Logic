#include <core/BooleanFunctionParser.hpp>
#include <Exceptions.hpp>
#include <vector>
#include <stdint.h>
#include <mutex>
#include <regex>

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
static string getInfixTokensRegex() {
    static string result;
    unique_lock<mutex> lock(infixTokenRegexMutex);
    if (result.length() == 0) {
        vector<string> tokens = OPERATOR_REGEXES;
        tokens.push_back("[\\(]");
        tokens.push_back("[\\)]");
        tokens.push_back("[a-zA-Z]+");
        result = "[\\s]*(" + // Optional leading spaces
                 join(tokens, "|") + // actual token regex
                 ")[\\s]*"; // optional trailing spaces
    }
    return result;
}

bool isValidVariable(const string &var) {
    return regex_match(var, regex(VARIABLE_REGEX));
}

void BooleanFunctionAccumulator::push(const string &variable) {
    BooleanFunction function(TruthTable({ variable }));
    function.getTruthTable()[0] = false;
    function.getTruthTable()[1] = true;
    _stack.push(function);
}

void BooleanFunctionAccumulator::push(const UnaryOperator<bool> &_operator) {
    if (_stack.empty()) {
        throw IllegalStateException("Cannot push a unary operator on an empty stack.");
    }
    _stack.push(topAndPop(_stack).operate(_operator));
}

void BooleanFunctionAccumulator::push(const BinaryOperator<bool> &_operator) {
    if (_stack.size() < 2) {
        throw IllegalStateException("Cannot push a binary operator on an stack of size less than 2");
    }
    BooleanFunction operand2 = topAndPop(_stack);
    BooleanFunction operand1 = topAndPop(_stack);
    _stack.push(operand1.operate(_operator, operand2));
}

BooleanFunction BooleanFunctionAccumulator::pop() {
    if (_stack.size() == 1) {
        return topAndPop(_stack);
    }

    throw IllegalStateException("Cannot finish accumulation, because the stack size is still > 1");
}

static vector<string> getInfixTokens(const string &function) {
    regex tokensRegex(getInfixTokensRegex());
    smatch sm;
    vector<string> tokens;
    uint64_t i = 0;
    // regex_search can't take an r-value. Need to store it in a temp variable
    string substring;
    while (i < function.length() && regex_search((substring = function.substr(i, string::npos)), sm, tokensRegex, regex_constants::match_continuous)) {
        // Index 1 => The first group in the regex
        tokens.push_back(sm[1]);
        i += (uint64_t) sm.position(1) + (uint64_t) sm[1].length();
    }
    if (i != function.length()) {
        // prematurely failed lookup because of match_continuous flag. Unknown token at index i
        throw UnknownTokenException("Unknown token in the string at index " + to_string(i));
    }
    return tokens;
}

static vector<string> getPostfixTokens(const string &function) {
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
                    break;
                }
                postfixTokens.push_back(top);
            }
        } else if (regex_match(token, sm, stackItemsRegex)) {
            operatorStack.push(token);
        } else {
            // Not ")" or "(", and not matching operator regexes, but still in the list of infix tokens
            // => must be a variable name
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
    vector<string> postfixTokens = getPostfixTokens(function);
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
            accumulator.push(token);
        }
    }

    try {
        return accumulator.pop();
    } catch (const IllegalStateException &e) {
        throw new BadBooleanFunctionException("Missing operator tokens in the boolean function.");
    }
}
}
