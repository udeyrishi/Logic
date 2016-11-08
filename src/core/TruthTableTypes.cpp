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

#include <core/TruthTableTypes.hpp>
#include <iostream>

namespace Logic {

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
}
