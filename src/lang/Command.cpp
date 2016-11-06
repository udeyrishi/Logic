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
BooleanFunction parse(const string &expression, const Runtime &runtime) {
    return BooleanFunctionParser().parse(expression, [&](const string &functionName) -> const BooleanFunction& {
        return runtime.get(functionName);
    });
}

bool CreateBooleanFunctionCommand::execute(const string &args, Runtime &runtime, ostream &out) {
    UNUSED(out);

    smatch sm;
    if (regex_match(args, sm, CREATE_ARGS_REGEX, regex_constants::match_continuous)) {
        string variableName = sm[1];
        string expression = sm[2];
        runtime.save(variableName, parse(expression, runtime));
        return true;
    }

    throw BadCommandArgumentsException("Unknown args to command 'let': " + args);
}

bool PrintBooleanFunctionCommand::execute(const string &expression, Runtime &runtime, ostream &out) {
    out << parse(expression, runtime) << endl;
    return true;
}

bool DeleteBooleanFunctionCommand::execute(const string &functionName, Runtime &runtime, ostream &out) {
    UNUSED(out);
    runtime.erase(functionName);
    return true;
}

bool PrintMaxtermsCommand::execute(const string &expression, Runtime &runtime, ostream &out) {
    out << join(parse(expression, runtime).getTruthTable().getMaxterms(), ", ") << endl;
    return true;
}

bool PrintMintermsCommand::execute(const string &expression, Runtime &runtime, ostream &out) {
    out << join(parse(expression, runtime).getTruthTable().getMinterms(), ", ") << endl;
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
    } else if (contains(MAXTERMS_COMMAND, command)) {
        return new PrintMaxtermsCommand();
    } else if (contains(MINTERMS_COMMAND, command)) {
        return new PrintMintermsCommand();
    }

    throw UnknownCommandException("Unknown command: " + command);
}
}
