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

#include <string>
#include <core/BooleanFunction.hpp>
#include <core/Utils.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace Logic {
class Runtime {
public:
    void save(const string &variableName, const BooleanFunction &function);
    BooleanFunction &get(const string &variableName);
    const BooleanFunction &get(const string &variableName) const;
    bool contains(const string &variableName) const;
    void erase(const string &variableName);

    void flag(const string &flagName);
    bool getFlag(const string &flagName);
    void clearFlags();
private:
    unordered_map<string, BooleanFunction> workspace;
    unordered_set<string> flags;
};
}
