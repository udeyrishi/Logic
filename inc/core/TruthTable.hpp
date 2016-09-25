#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <stdint.h>
#include <stdexcept>

using namespace std;

namespace Logic {
#define MAX_NUM_VARIABLES 64

class __TruthTableValueProxy;

typedef uint64_t TruthTableUInt;

class TruthTableVariablesUInt {
public:
    TruthTableVariablesUInt(const uint8_t value) : value(value) {
        assertFits<out_of_range>(value);
    }

    TruthTableVariablesUInt() : value(0) {
    }

    operator uint8_t() const {
        return value;
    }

    TruthTableVariablesUInt &operator++();
    TruthTableVariablesUInt operator++(int);
    TruthTableVariablesUInt &operator--();
    TruthTableVariablesUInt operator--(int);
    TruthTableVariablesUInt &operator+=(const TruthTableVariablesUInt &rhs);
    TruthTableVariablesUInt &operator-=(const TruthTableVariablesUInt &rhs);

    // Using this instead of templates so as to use the "==" operator
    bool operator==(const TruthTableVariablesUInt &rhs) const;
    bool operator==(const int32_t &rhs) const;
    bool operator==(const int64_t &rhs) const;
    bool operator==(const uint32_t &rhs) const;
    bool operator==(const uint64_t &rhs) const;

    template <typename TException>
    static void assertFits(const uint64_t value);
private:
    uint8_t value;
};

// Using this instead of templates so as to use the "+/-" operators
TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const TruthTableVariablesUInt &rhs);
TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const int32_t &rhs);
TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const int64_t &rhs);
TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const uint32_t &rhs);
TruthTableVariablesUInt operator+(const TruthTableVariablesUInt &lhs, const uint64_t &rhs);
TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const TruthTableVariablesUInt &rhs);
TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const int32_t &rhs);
TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const int64_t &rhs);
TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const uint32_t &rhs);
TruthTableVariablesUInt operator-(const TruthTableVariablesUInt &lhs, const uint64_t &rhs);
ostream &operator<<(ostream &os, const TruthTableVariablesUInt &val);
istream &operator>>(istream &is, TruthTableVariablesUInt &val);

class TruthTable {
public:
    TruthTable(const vector<string> &variables);

    const vector<string> &getVariables() const {
        return variables;
    }

    TruthTableUInt size() const {
        return (TruthTableUInt) pow(2, variables.size());
    }

    __TruthTableValueProxy operator[](const TruthTableUInt index);
    bool operator[](const TruthTableUInt index) const;

    static bool getVariableValueInLine(TruthTableVariablesUInt columnNumber, TruthTableUInt lineIndex);
private:
    vector<string> variables;
    vector<bool> values;

    void validateIndex(const TruthTableUInt index) const;

    friend class __TruthTableValueProxy;
};

ostream &operator<<(ostream &os, const TruthTable &table);

// This type should not be used directly by keeping a variable. It should be either converted to a bool, or used for assignment.
class __TruthTableValueProxy {
public:
    __TruthTableValueProxy(const TruthTableUInt index, TruthTable &table) : index(index), table(table) {
    }

    operator bool() const {
        return table.values[index];
    }

    void operator=(const bool value) {
        table.values[index] = value;
    }

private:
    const TruthTableUInt index;
    TruthTable &table;
};

ostream &operator<<(ostream &os, const __TruthTableValueProxy &val);

class TruthTableBuilder {
public:
    void set(TruthTableUInt lineIndex, const bool b);

    void setVariables(const vector<string> &variables) {
        this->variables = variables;
    }

    TruthTable build() const;

private:
    vector<bool> values;
    vector<string> variables;
};
}

namespace std {
template<>
struct hash<Logic::TruthTableVariablesUInt> {
    size_t operator()(const Logic::TruthTableVariablesUInt &val) const {
        return (uint8_t)val;
    }
};
}