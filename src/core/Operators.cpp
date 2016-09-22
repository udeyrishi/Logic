#include <core/Operators.hpp>

using namespace std;

namespace Logic {

bool isKnownUnaryOperator(const string &_operator) {
    return _operator == NOT_SYMBOL;
}

bool isKnownBinaryOperator(const string &_operator) {
    return _operator == AND_SYMBOL ||
           _operator == OR_SYMBOL  ||
           _operator == XOR_SYMBOL;
}

template <typename T>
UnaryOperator<T> getUnaryOperatorWithSymbol(const string &_operator) {
    if (_operator == NOT_SYMBOL) {
        return Not<T>();
    }
    throw invalid_argument("Unknown operator: " + _operator);
}

template <typename T>
BinaryOperator<T> getBinaryOperatorWithSymbol(const string &_operator) {
    if (_operator == AND_SYMBOL) {
        return And<T>();
    } else if (_operator == OR_SYMBOL) {
        return Or<T>();
    } else if (_operator == XOR_SYMBOL) {
        return Xor<T>();
    }
    throw invalid_argument("Unknown operator: " + _operator);
}
}
