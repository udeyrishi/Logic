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
#include <Exceptions.hpp>
#include <Utils.hpp>

using namespace std;

static const regex CREATE_ARGS_REGEX("\\s*([a-zA-Z]+)\\s*[=]\\s*(.+)\\s*");

namespace Logic {
void CreateBooleanFunctionCommand::execute(const string &args, Runtime &runtime, ostream &out) {
    UNUSED(out);
    smatch sm;
    if (regex_match(args, sm, CREATE_ARGS_REGEX, regex_constants::match_continuous)) {
        string variableName = sm[1];
        BooleanFunctionParser functionParser;
        runtime.save(variableName, functionParser.parse(sm[2]));
    } else {
        throw BadCommandArgumentsException("Unknown args to command 'let': " + args);
    }
}

void PrintBooleanFunctionCommand::execute(const string &functionName, Runtime &runtime, ostream &out) {
    if (runtime.contains(functionName)) {
        out << runtime.get(functionName);
    } else {
        out << "Not found: " + functionName;
    }
}

Command *getCommand(const string &command) {
    if (command == CREATE_BOOLEAN_FUNCTION_COMMAND) {
        return new CreateBooleanFunctionCommand();
    } else if (command == PRINT_BOOLEAN_FUNCTION_COMMAND) {
        return new PrintBooleanFunctionCommand();
    }

    throw UnknownCommandException("Unknown command: " + command);
}
}
