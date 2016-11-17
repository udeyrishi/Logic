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

#define REGISTER_COMMAND(COMMAND, ...) \
    dispatchTable.registerCommand({ __VA_ARGS__ }, []() { return unique_ptr<Command>(new COMMAND##Command()); });

inline DispatchTable createDispatchTableWithAllCommands() {
    DispatchTable dispatchTable;
    // See lang/Command.hpp for the list of available commands
    REGISTER_COMMAND(Quit, "quit", "q");
    REGISTER_COMMAND(CreateBooleanFunction, "let", "l");
    REGISTER_COMMAND(PrintBooleanFunction, "print", "p");
    REGISTER_COMMAND(DeleteBooleanFunction, "delete", "d");
    REGISTER_COMMAND(PrintMinterms, "minterms", "min");
    REGISTER_COMMAND(PrintMaxterms, "maxterms", "max");
    REGISTER_COMMAND(PrintVariables, "variables", "v");
    REGISTER_COMMAND(If, "if");
    return dispatchTable;
}
}
