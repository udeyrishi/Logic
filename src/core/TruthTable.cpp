#include <core/TruthTable.hpp>
#include <iostream>

using namespace Logic;

bool isPowerOfTwo(TruthTableUInt n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

template <typename TException>
void TruthTableVariablesUInt::assertFits(const uint64_t value) {
    if (value > MAX_NUM_VARIABLES) {
        throw TException("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator++() {
    ++value;
    assertFits<overflow_error>(value);
    return *this;
}

TruthTableVariablesUInt TruthTableVariablesUInt::operator++(int) {
    TruthTableVariablesUInt temp(*this);
    ++value;
    assertFits<overflow_error>(value);
    return temp;
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator--() {
    if (value == 0) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    --value;
    return *this;
}

TruthTableVariablesUInt TruthTableVariablesUInt::operator--(int) {
    if (value == 0) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    TruthTableVariablesUInt temp(*this);
    --value;
    return temp;
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator+=(const TruthTableVariablesUInt &rhs) {
    uint32_t added = (uint32_t) value + (uint32_t) rhs.value;
    assertFits<overflow_error>(added);
    value = (uint8_t) added;
    return *this;
}

TruthTableVariablesUInt &TruthTableVariablesUInt::operator-=(const TruthTableVariablesUInt &rhs) {
    if (rhs.value > value) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    value = value - rhs.value;
    return *this;
}

bool TruthTableVariablesUInt::operator==(const TruthTableVariablesUInt &rhs) const {
    return value == rhs.value;
}

bool TruthTableVariablesUInt::operator==(const int32_t &rhs) const {
    return (int32_t) value == rhs;
}

bool TruthTableVariablesUInt::operator==(const int64_t &rhs) const {
    return (int64_t) value == rhs;
}

bool TruthTableVariablesUInt::operator==(const uint32_t &rhs) const {
    return (uint32_t) value == rhs;
}

bool TruthTableVariablesUInt::operator==(const uint64_t &rhs) const {
    return (uint64_t) value == rhs;
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
    return lhs + (uint64_t) rhs;
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const int32_t &rhs) {
    return lhs + (int64_t) rhs;
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const int64_t &rhs) {
    int64_t added = (int64_t) lhs + rhs;
    if (added < 0) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    TruthTableVariablesUInt::assertFits<overflow_error>((uint64_t) added);
    return TruthTableVariablesUInt((uint8_t) added);
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const uint32_t &rhs) {
    return lhs + (uint64_t) rhs;
}

TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const uint64_t &rhs) {
    uint64_t added = (uint8_t) lhs + rhs;
    TruthTableVariablesUInt::assertFits<overflow_error>(added);
    return TruthTableVariablesUInt((uint8_t) added);
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const TruthTableVariablesUInt &rhs) {
    return lhs - (uint64_t) rhs;
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const int32_t &rhs) {
    return lhs - (int64_t) rhs;
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const int64_t &rhs) {
    int64_t subtracted = (int64_t) lhs - rhs;
    if (subtracted < 0) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    TruthTableVariablesUInt::assertFits<overflow_error>((uint64_t) subtracted);
    return TruthTableVariablesUInt((uint8_t) subtracted);
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const uint32_t &rhs) {
    return lhs - (uint64_t) rhs;
}

TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const uint64_t &rhs) {
    if (rhs > lhs) {
        throw underflow_error("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }

    return TruthTableVariablesUInt((uint8_t) lhs - (uint8_t) rhs);
}

ostream &operator<<(ostream &os, const TruthTableVariablesUInt &val) {
    os << to_string(val);
    return os;
}

istream &operator>>(istream &is, TruthTableVariablesUInt &val) {
    int64_t intVal;
    is >> intVal;
    if (intVal < 0 || intVal > MAX_NUM_VARIABLES) {
        throw out_of_range("Value needs to be in range: 0 <= x <= " + to_string(MAX_NUM_VARIABLES));
    }
    val = (uint8_t) intVal;
    return is;
}

ostream &operator<<(ostream &os, const __TruthTableValueProxy &val) {
    os << (bool) val;
    return os;
}
}
