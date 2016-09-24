#pragma once

#include <string>
#include <core/BooleanFunction.hpp>
#include <Utils.hpp>

using namespace std;

namespace Logic {
class Runtime
{
public:
    void save(const string variableName, BooleanFunction function) {
        UNUSED(variableName);
        UNUSED(function);
    }
};
}