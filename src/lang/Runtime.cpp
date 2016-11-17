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

#include <lang/Runtime.hpp>
#include <lang/Exceptions.hpp>

using namespace std;

namespace Logic {
void Runtime::save(const string &variableName, const BooleanFunction &function) {
    const auto found = workspace.find(variableName);
    if (found == workspace.end()) {
        workspace.insert(make_pair(variableName, function));
    } else {
        found->second = function;
    }
}

BooleanFunction &Runtime::get(const string &variableName) {
    const auto found = workspace.find(variableName);
    if (found == workspace.end()) {
        throw BooleanFunctionNotFoundException("Boolean Function not found in the current workspace: " + variableName);
    }
    return found->second;
}

const BooleanFunction &Runtime::get(const string &variableName) const {
    const auto found = workspace.find(variableName);
    if (found == workspace.end()) {
        throw BooleanFunctionNotFoundException("Boolean Function not found in the current workspace: " + variableName);
    }
    return found->second;
}

bool Runtime::contains(const string &variableName) const {
    return workspace.find(variableName) != workspace.end();
}

void Runtime::erase(const string &variableName) {
    const auto found = workspace.find(variableName);
    if (found == workspace.end()) {
        throw BooleanFunctionNotFoundException("Boolean Function not found in the current workspace: " + variableName);
    }
    workspace.erase(found);
}

void Runtime::flag(const string &flagName) {
    flags.insert(flagName);
}

bool Runtime::getFlag(const string &flagName) {
    const auto found = flags.find(flagName);
    const bool flagged = found != flags.end();
    if (flagged) {
        flags.erase(found);
    }
    return flagged;
}

void Runtime::clearFlags() {
    flags.clear();
}

}
