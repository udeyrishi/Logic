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

#include <lang/Command.hpp>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>
#include <memory>

using namespace std;

namespace Logic {
class DispatchTable {
public:
    void registerCommand(const vector<string> &symbols, function<unique_ptr<Command> ()> commandFactory);
    unique_ptr<Command> getCommand(const string &token);

private:
    unordered_map<string, function<unique_ptr<Command> ()>> table;
};

DispatchTable createDispatchTableWithAllCommands();
}
