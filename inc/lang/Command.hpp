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
#include <lang/Runtime.hpp>
#include <iostream>
#include <functional>

using namespace std;

#define DECLARE_COMMAND_CLASS(COMMAND_NAME)                                                       \
class COMMAND_NAME##Command : public Command {                                                    \
public:                                                                                           \
    virtual bool execute(const string &, Runtime &, ostream &, function<bool (istream &)>);           \
}

namespace Logic {
class Command {
public:
    virtual bool execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) = 0;
    virtual ~Command() {
    }
};

DECLARE_COMMAND_CLASS(Quit);
DECLARE_COMMAND_CLASS(CreateBooleanFunction);
DECLARE_COMMAND_CLASS(PrintBooleanFunction);
DECLARE_COMMAND_CLASS(DeleteBooleanFunction);
DECLARE_COMMAND_CLASS(PrintMinterms);
DECLARE_COMMAND_CLASS(PrintMaxterms);
DECLARE_COMMAND_CLASS(PrintVariables);
DECLARE_COMMAND_CLASS(If);
DECLARE_COMMAND_CLASS(While);
// When adding new commands, update createDispatchTableWithAllCommands() in DispatchTable.hpp
// to ensure that the command is available at runtime.
}
