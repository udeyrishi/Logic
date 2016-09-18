#include <core/TruthTable.hpp>
#include <iostream>

using namespace Logic;

bool isPowerOfTwo(TruthTableUInt n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

void TruthTableVariablesUInt::assertFits(const TruthTableUInt value) {
    if (value > MAX_NUM_VARIABLES) {
        throw out_of_range("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator++() {
    ++value;
    assertFits(value);
    return *this;
}

TruthTableVariablesUInt TruthTableVariablesUInt::operator++(int) {
    TruthTableVariablesUInt temp(*this);
    ++value;
    assertFits(value);
    return temp;
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator+=(const TruthTableVariablesUInt &rhs) {
    uint32_t added = (uint32_t) value + (uint32_t) rhs.value;
    assertFits(added);
    value = (uint8_t) added;
    return *this;
}

bool TruthTableVariablesUInt::operator==(const TruthTableVariablesUInt &rhs) const {
    return value == rhs.value;
}

TruthTable::TruthTable(const vector<string> &variables) : variables(variables), values(variables.size(), false) {
    if (variables.size() == 0 || variables.size() > MAX_NUM_VARIABLES) {
        throw invalid_argument("variables' size needs to be 0 < n <= " + to_string(MAX_NUM_VARIABLES));
    }
}

__TruthTableValueProxy TruthTable::operator[](const TruthTableUInt index) {
    validateIndex(index);
    return __TruthTableValueProxy(index, *this);
}

bool TruthTable::operator[](const TruthTableUInt index) const {
    validateIndex(index);
    return values[index];
}

void TruthTable::validateIndex(const TruthTableUInt index) const {
    if (index >= size()) {
        throw out_of_range("index needs to be in range: [0, " + to_string(size() - 1) + "]");
    }
}

TruthTable TruthTableBuilder::build() const {
    if (values.size() <= 1 || !isPowerOfTwo(values.size())) {
        throw IllegalTruthTableException("Number of lines needs to be a power of 2 that's greater than 1.");
    }

    if ((TruthTableUInt) pow(2, variables.size()) != values.size()) {
        throw IllegalTruthTableException("Number of lines should be 2**number of variables.");
    }

    TruthTable built(variables);
    for (TruthTableUInt i = 0; i < values.size(); ++i) {
        built[i] = values[i];
    }

    return built;
}

void TruthTableBuilder::set(TruthTableUInt lineIndex, const bool b) {
    for (TruthTableUInt i = values.size(); i <= lineIndex; ++i) {
        values.push_back(false);
    }

    values[lineIndex] = b;
}

namespace Logic {
    TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const TruthTableVariablesUInt &rhs) {
        uint32_t added = (uint8_t) lhs + (uint8_t) rhs;
        TruthTableVariablesUInt::assertFits(added);
        return TruthTableVariablesUInt((uint8_t) added);
    }

    TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const TruthTableVariablesUInt &rhs) {
        if (rhs > lhs) {
            throw underflow_error("lhs - rhs leads to a negative number. Not allowed.");
        }

        uint32_t subtracted = (uint8_t) lhs - (uint8_t) rhs;
        TruthTableVariablesUInt::assertFits(subtracted);
        return TruthTableVariablesUInt((uint8_t) subtracted);
    }

    ostream &operator<<(ostream &os, const TruthTableVariablesUInt &val) {
        os << to_string(val);
        return os;
    }

    istream &operator>>(istream &is, TruthTableVariablesUInt &val) {
        uint32_t intVal;
        is >> intVal;
        TruthTableVariablesUInt::assertFits(intVal);
        val = (uint8_t) intVal;
        return is;
    }

    ostream &operator<<(ostream &os, const __TruthTableValueProxy &val) {
        os << (bool) val;
        return os;
    }
}
