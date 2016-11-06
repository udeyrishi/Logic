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

using namespace std;

namespace Logic {
class Command {
public:
    virtual bool execute(const string &args, Runtime &runtime, ostream &out) = 0;
    virtual ~Command() {
    }
};

static const vector<string> CREATE_COMMAND({"let", "l"});
class CreateBooleanFunctionCommand : public Command  {
public:
    virtual bool execute(const string &args, Runtime &runtime, ostream &out);
};

static const vector<string> PRINT_COMMAND({"print", "p"});
class PrintBooleanFunctionCommand : public Command {
public:
    virtual bool execute(const string &expression, Runtime &runtime, ostream &out);
};

static const vector<string> DELETE_COMMAND({"delete", "d"});
class DeleteBooleanFunctionCommand : public Command {
    virtual bool execute(const string &functionName, Runtime &runtime, ostream &out);
};

static const vector<string> MINTERMS_COMMAND({"minterms", "min"});
class PrintMintermsCommand : public Command {
    virtual bool execute(const string &expression, Runtime &runtime, ostream &out);
};

static const vector<string> MAXTERMS_COMMAND({"maxterms", "max"});
class PrintMaxtermsCommand : public Command {
    virtual bool execute(const string &expression, Runtime &runtime, ostream &out);
};

static const vector<string> VARIABLES_COMMAND({"variables", "v"});
class PrintVariablesCommand : public Command {
    virtual bool execute(const string &expression, Runtime &runtime, ostream &out);
};

static const vector<string> QUIT_COMMAND({"quit", "q"});
class QuitCommand : public Command {
    virtual bool execute(const string &args, Runtime &runtime, ostream &out);
};

Command *getCommand(const string &command);
}
