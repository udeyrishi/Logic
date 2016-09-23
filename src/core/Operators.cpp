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
}
