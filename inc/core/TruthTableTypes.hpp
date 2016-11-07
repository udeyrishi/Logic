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

#pragma once

#include <stdint.h>
#include <stdexcept>
#include <unordered_map>

using namespace std;

#define MAX_NUM_VARIABLES 64

namespace Logic {
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
}

namespace std {
template<>
struct hash<Logic::TruthTableVariablesUInt> {
    size_t operator()(const Logic::TruthTableVariablesUInt &val) const {
        return (uint8_t)val;
    }
};
}
