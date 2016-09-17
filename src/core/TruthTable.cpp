#include <core/TruthTable.hpp>
#include <iostream>

using namespace Logic;

void validateNumVariables(uint64_t numVariables) {
    if (numVariables == 0) {
        throw invalid_argument("variables' size needs to be non-zero");
    }

    if (numVariables > 32) {
        throw invalid_argument("Currently only supporting a maximum of 32 variables");
    }
}

bool isPowerOfTwo(uint64_t n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

TruthTable::TruthTable(const vector<string> &variables) : variables(variables), values(variables.size(), false) {
    validateNumVariables(variables.size());
}

__TruthTableValueProxy TruthTable::operator[](const uint64_t index) {
    validateIndex(index);
    return __TruthTableValueProxy(index, *this);
}

bool TruthTable::operator[](const uint64_t index) const {
    validateIndex(index);
    return values[index];
}

void TruthTable::validateIndex(const uint64_t index) const {
    if (index >= size()) {
        throw out_of_range("index needs to be in range: [0, " + to_string(size() -1 ) + "]");
    }
}

ostream &operator<<(ostream &os, __TruthTableValueProxy &val) {
    os << (bool) val;
    return os;
}

TruthTable TruthTableBuilder::build() const {
    if (values.size() <= 1 || !isPowerOfTwo(values.size())) {
        throw IllegalTruthTableException("Number of lines needs to be a power of 2 that's greater than 1.");
    }

    if ((uint64_t) pow(2, variables.size()) != values.size()) {
        throw IllegalTruthTableException("Number of lines should be 2**number of variables.");
    }

    TruthTable built(variables);
    for (uint64_t i = 0; i < values.size(); ++i) {
        built[i] = values[i];
    }

    return built;
}

void TruthTableBuilder::set(uint64_t lineIndex, const bool b) {
    for (uint64_t i = values.size(); i <= lineIndex; ++i) {
        values.push_back(false);
    }

    values[lineIndex] = b;
}
