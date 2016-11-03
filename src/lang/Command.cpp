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

#include <lang/Command.hpp>
#include <regex>
#include <core/BooleanFunctionParser.hpp>
#include <lang/Exceptions.hpp>
#include <core/Utils.hpp>
#include <algorithm>

using namespace std;

static const regex CREATE_ARGS_REGEX("\\s*([a-zA-Z]+)\\s*[=]\\s*(.+)\\s*");

namespace Logic {
bool CreateBooleanFunctionCommand::execute(const string &args, Runtime &runtime, ostream &out) {
    UNUSED(out);
    smatch sm;
    if (regex_match(args, sm, CREATE_ARGS_REGEX, regex_constants::match_continuous)) {
        string variableName = sm[1];
        BooleanFunctionParser functionParser;
        runtime.save(variableName, functionParser.parse(sm[2], [&](const string &functionName) -> const BooleanFunction& {
            return runtime.get(functionName);
        }));
        return true;
    } else {
        throw BadCommandArgumentsException("Unknown args to command 'let': " + args);
    }
}

bool PrintBooleanFunctionCommand::execute(const string &expression, Runtime &runtime, ostream &out) {
    out << BooleanFunctionParser().parse(expression, [&](const string &functionName) -> const BooleanFunction& {
        return runtime.get(functionName);
    });
    return true;
}

bool DeleteBooleanFunctionCommand::execute(const string &functionName, Runtime &runtime, ostream &out) {
    UNUSED(out);
    runtime.erase(functionName);
    return true;
}

bool QuitCommand::execute(const string &args, Runtime &runtime, ostream &out) {
    UNUSED(out);
    UNUSED(runtime);
    if (args.length() != 0) {
        throw BadCommandArgumentsException("Unknown args to command 'quit': " + args);
    }
    return false;
}

Command *getCommand(const string &command) {
    if (contains(CREATE_COMMAND, command)) {
        return new CreateBooleanFunctionCommand();
    } else if (contains(PRINT_COMMAND, command)) {
        return new PrintBooleanFunctionCommand();
    } else if (contains(DELETE_COMMAND, command)) {
        return new DeleteBooleanFunctionCommand();
    } else if (contains(QUIT_COMMAND, command)) {
        return new QuitCommand();
    }

    throw UnknownCommandException("Unknown command: " + command);
}
}
