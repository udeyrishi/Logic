#pragma once

#include <string>
#include <core/BooleanFunction.hpp>
#include <Utils.hpp>
#include <unordered_map>

using namespace std;

namespace Logic {
class Runtime
{
public:
    void save(const string &variableName, const BooleanFunction &function);
    BooleanFunction &get(const string &variableName);
    bool contains(const string &variableName) const;
private:
    unordered_map<string, BooleanFunction> workspace;
};
}