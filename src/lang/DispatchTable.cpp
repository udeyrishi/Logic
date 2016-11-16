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

#include <lang/DispatchTable.hpp>
#include <lang/Exceptions.hpp>

using namespace std;

namespace Logic {
void DispatchTable::registerCommand(const vector<string> &symbols, function<unique_ptr<Command> ()> commandFactory) {
    for (const string &symbol : symbols) {
        table[symbol] = commandFactory;
    }
}

unique_ptr<Command> DispatchTable::getCommand(const string &token) {
    const auto search = table.find(token);
    if (search == table.end()) {
        throw UnknownCommandException("Unknown command: " + token);
    }
    return search->second();
}
}
