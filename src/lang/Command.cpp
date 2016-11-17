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
#include <sstream>
#include <utility>

using namespace std;

namespace Logic {
static const regex CREATE_ARGS_REGEX("\\s*(" + VARIABLE_REGEX + ")\\s*[=]\\s*(.+)\\s*");

BooleanFunction parse(const string &expression, const Runtime &runtime) {
    return BooleanFunctionParser().parse(expression, [&](const string &functionName) -> const BooleanFunction& {
        return runtime.get(functionName);
    });
}

bool QuitCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(out);
    UNUSED(runtime);
    UNUSED(interpreter);

    if (args.length() != 0) {
        throw BadCommandArgumentsException("Unknown args to command 'quit': " + args);
    }
    return false;
}

bool CreateBooleanFunctionCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(out);
    UNUSED(interpreter);

    smatch sm;
    if (regex_match(args, sm, CREATE_ARGS_REGEX, regex_constants::match_continuous)) {
        string variableName = sm[1];
        string expression = sm[2];
        runtime.save(variableName, parse(expression, runtime));
        return true;
    }

    throw BadCommandArgumentsException("Unknown args to command 'let': " + args);
}

bool PrintBooleanFunctionCommand::execute(const string &expression, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(interpreter);

    out << parse(expression, runtime) << endl;
    return true;
}

bool DeleteBooleanFunctionCommand::execute(const string &functionName, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(interpreter);

    UNUSED(out);
    runtime.erase(functionName);
    return true;
}

bool PrintMaxtermsCommand::execute(const string &expression, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(interpreter);

    out << join(parse(expression, runtime).getTruthTable().getMaxterms(), ", ") << endl;
    return true;
}

bool PrintMintermsCommand::execute(const string &expression, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(interpreter);

    out << join(parse(expression, runtime).getTruthTable().getMinterms(), ", ") << endl;
    return true;
}

bool PrintVariablesCommand::execute(const string &expression, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(interpreter);

    vector<string> variables = parse(expression, runtime).getTruthTable().getVariables();
    // This is how the variables are shown in the truth table -- little endian
    reverse(variables.begin(), variables.end());
    out << join(variables, ", ") << endl;
    return true;
}

static pair<const string, const string> parseScopeCommandArgs(const string &args, const string &commandName) {
    static const regex conditionRegex("[\\s]*(.+?)[\\s]*[\\{]{1}[\\s]*(.+)[\\s]*[\\}]{1}");
    smatch sm;

    if (!regex_search(args, sm, conditionRegex, regex_constants::match_continuous)) {
        throw BadCommandArgumentsException("Unknown args to command '" + commandName + "': " + args);
    }

    return make_pair(sm[1], sm[2]);
}

bool IfCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(out);
    const auto scopeCommandArgs = parseScopeCommandArgs(args, "if");

    BooleanFunction conditionFunction = parse(scopeCommandArgs.first, runtime);
    if (!conditionFunction.isConstant()) {
        throw BadCommandArgumentsException("The condition to the 'if' command needs to evaluate to a constant value Boolean function.");
    }

    if (conditionFunction.getConstantValue()) {
        stringstream ss;
        ss << scopeCommandArgs.second;
        return interpreter(ss);
    }

    return true;
}

bool WhileCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(out);
    const auto scopeCommandArgs = parseScopeCommandArgs(args, "while");

    while (true) {
        BooleanFunction conditionFunction = parse(scopeCommandArgs.first, runtime);
        if (!conditionFunction.isConstant()) {
            throw BadCommandArgumentsException("The condition to the 'while' command needs to evaluate to a constant value Boolean function.");
        }

        if (!conditionFunction.getConstantValue()) {
            break;
        }

        stringstream ss;
        ss << scopeCommandArgs.second;
        if (!interpreter(ss)) {
            return false;
        }
    }

    return true;
}
}
