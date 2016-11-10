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

#include <core/BooleanFunction.hpp>
#include <core/Exceptions.hpp>
#include <iostream>

using namespace std;

namespace Logic {

ostream &operator<<(ostream &os, const BooleanFunction &function) {
    if (function.isConstant()) {
        os << (function.getConstantValue() ? 1 : 0);
    } else {
        os << function.getTruthTable();
    }

    return os;
}

bool operator==(const BooleanFunction &left, const BooleanFunction &right) {
    if (left.isConstant() && right.isConstant()) {
        return left.getConstantValue() == right.getConstantValue();
    }

    if (left.hasTruthTable() && right.hasTruthTable()) {
        return left.getTruthTable() == right.getTruthTable();
    }

    return false;
}

BooleanFunction::BooleanFunction(const TruthTable &table) : constValue(nullptr) {
    this->table = new TruthTable(table);
}

BooleanFunction::BooleanFunction(const bool constValue) : table(nullptr) {
    this->constValue = new bool(constValue);
}

BooleanFunction::BooleanFunction(const BooleanFunction &rhs) {
    init(rhs);
}

BooleanFunction &BooleanFunction::operator=(const BooleanFunction &rhs) {
    if (this != &rhs) {
        destroy();
        init(rhs);
    }
    return *this;
}

BooleanFunction::~BooleanFunction() {
    destroy();
}

void BooleanFunction::init(const BooleanFunction &rhs) {
    this->constValue = rhs.constValue == nullptr ? nullptr : new bool(*rhs.constValue);
    this->table = rhs.table == nullptr ? nullptr : new TruthTable(*rhs.table);
}

void BooleanFunction::destroy() {
    if (constValue != nullptr) {
        delete constValue;
        constValue = nullptr;
    }

    if (table != nullptr) {
        delete table;
        table = nullptr;
    }
}

bool BooleanFunction::hasTruthTable() const {
    return table != nullptr;
}

TruthTable &BooleanFunction::getTruthTable() {
    if (hasTruthTable()) {
        return *table;
    }

    throw IllegalStateException("Cannot get the truth table of a constant value Boolean function.");
}

const TruthTable &BooleanFunction::getTruthTable() const {
    if (hasTruthTable()) {
        return *table;
    }

    throw IllegalStateException("Cannot get the truth table of a constant value Boolean function.");
}

bool BooleanFunction::isConstant() const {
    return constValue != nullptr;
}

bool BooleanFunction::getConstantValue() const {
    if (isConstant()) {
        return *constValue;
    }

    throw IllegalStateException("Cannot get the constant value of a non-constant Boolean function.");
}

bool &BooleanFunction::getConstantValue() {
    if (isConstant()) {
        return *constValue;
    }

    throw IllegalStateException("Cannot get the constant value of a non-constant Boolean function.");
}
}
