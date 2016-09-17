#include <core/TruthTable.hpp>
#include <iostream>

using namespace Logic;

__TruthTableValueProxy TruthTable::operator[](const int index) {
    if (index >= size() || index < 0) {
        throw out_of_range("index needs to be in range: [0, " + to_string(size() -1 ) + "]");
    }
    return __TruthTableValueProxy(index, *this);
}

ostream &operator<<(ostream &os, __TruthTableValueProxy &val) {
    os << (bool) val;
    return os;
}

