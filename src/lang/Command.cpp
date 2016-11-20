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
#include <core/Operators.hpp>
#include <algorithm>
#include <sstream>
#include <utility>

using namespace std;

namespace Logic {
static const string ELSE_ALLOWED = "else_allowed";
static const string RUN_ELSE = "run_else";

BooleanFunction parse(const string &expression, const Runtime &runtime) {
    return BooleanFunctionParser().parse(expression, [&](const string &functionName) -> const BooleanFunction& {
        return runtime.get(functionName);
    });
}

bool Command::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    UNUSED(args);
    UNUSED(out);
    UNUSED(interpreter);
    runtime.clearFlags();
    return true;
}

bool QuitCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(args, runtime, out, interpreter);
    UNUSED(out);
    UNUSED(runtime);
    UNUSED(interpreter);

    if (args.length() != 0) {
        throw BadCommandArgumentsException("Unknown args to command 'quit': " + args);
    }
    return false;
}

bool CreateBooleanFunctionCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(args, runtime, out, interpreter);
    UNUSED(out);
    UNUSED(interpreter);

    static const regex createArgsRegex("\\s*(.+?)\\s*[=]\\s*(.+)\\s*");
    smatch sm;
    if (regex_match(args, sm, createArgsRegex, regex_constants::match_continuous)) {
        string lhs = sm[1];
        string rhs = sm[2];

        static regex variableNameRegex(VARIABLE_REGEX);
        sm = smatch();
        if (regex_match(lhs, sm, variableNameRegex, regex_constants::match_continuous)) {
            runtime.save(lhs, parse(rhs, runtime));
            return true;
        }

        static regex indexAccessRegex("[\\$]{1}(" + VARIABLE_REGEX + ")" + "[\\s]*" + INDEX_REGEX + "[\\s]*");
        sm = smatch();
        if (regex_match(lhs, sm, indexAccessRegex, regex_constants::match_continuous)) {
            string variableName = sm[1];
            TruthTableUInt truthTableIndex = stoul(sm[2]);
            runtime.get(variableName).getTruthTable()[truthTableIndex] = parse(rhs, runtime).getConstantValue();
            return true;
        }
    }

    throw BadCommandArgumentsException("Unknown args to command 'let': " + args);
}

bool PrintBooleanFunctionCommand::execute(const string &expression, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(expression, runtime, out, interpreter);
    UNUSED(interpreter);

    out << parse(expression, runtime) << endl;
    return true;
}

bool DeleteBooleanFunctionCommand::execute(const string &functionName, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(functionName, runtime, out, interpreter);
    UNUSED(interpreter);
    UNUSED(out);

    runtime.erase(functionName);
    return true;
}

bool PrintMaxtermsCommand::execute(const string &expression, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(expression, runtime, out, interpreter);
    UNUSED(interpreter);

    out << join(parse(expression, runtime).getTruthTable().getMaxterms(), ", ") << endl;
    return true;
}

bool PrintMintermsCommand::execute(const string &expression, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(expression, runtime, out, interpreter);
    UNUSED(interpreter);

    out << join(parse(expression, runtime).getTruthTable().getMinterms(), ", ") << endl;
    return true;
}

bool PrintVariablesCommand::execute(const string &expression, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(expression, runtime, out, interpreter);
    UNUSED(interpreter);

    vector<string> variables = parse(expression, runtime).getTruthTable().getVariables();
    // This is how the variables are shown in the truth table -- little endian
    reverse(variables.begin(), variables.end());
    out << join(variables, ", ") << endl;
    return true;
}

static const string BLOCK_REGEX = "[\\s]*[\\{]{1}[\\s]*(.*)[\\s]*[\\}]{1}[\\s]*";

static pair<string, string> getConditionalCommandArgs(const string &args, const string &commandName) {
    static const regex conditionRegex("[\\s]*(.+?)[\\s]*" + BLOCK_REGEX);
    smatch sm;

    if (!regex_search(args, sm, conditionRegex, regex_constants::match_continuous)) {
        throw BadCommandArgumentsException("Unknown args to command '" + commandName + "': " + args);
    }

    return make_pair(sm[1], sm[2]);
}

bool IfCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(args, runtime, out, interpreter);
    UNUSED(out);

    const auto parsedArgs = getConditionalCommandArgs(args, "if");
    BooleanFunction conditionFunction = parse(parsedArgs.first, runtime);
    if (!conditionFunction.isConstant()) {
        throw BadCommandArgumentsException("The condition to the 'if' command needs to evaluate to a constant value Boolean function.");
    }

    bool _continue = true;
    if (conditionFunction.getConstantValue()) {
        stringstream ss;
        ss << parsedArgs.second;
        _continue = interpreter(ss);
        // A nested if condition could've failed and set a flag for else. Now it's not needed
        runtime.getFlag(RUN_ELSE);
    } else {
        runtime.flag(RUN_ELSE);
    }

    runtime.flag(ELSE_ALLOWED);
    return _continue;
}

bool WhileCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    Command::execute(args, runtime, out, interpreter);
    UNUSED(out);

    const auto parsedArgs = getConditionalCommandArgs(args, "while");

    while (true) {
        BooleanFunction conditionFunction = parse(parsedArgs.first, runtime);
        if (!conditionFunction.isConstant()) {
            throw BadCommandArgumentsException("The condition to the 'while' command needs to evaluate to a constant value Boolean function.");
        }

        if (!conditionFunction.getConstantValue()) {
            break;
        }

        stringstream ss;
        ss << parsedArgs.second;
        if (!interpreter(ss)) {
            return false;
        }
    }

    return true;
}

bool ElseCommand::execute(const string &args, Runtime &runtime, ostream &out, function<bool (istream &)> interpreter) {
    if (!runtime.getFlag(ELSE_ALLOWED)) {
        throw CommandNotAllowedException("'else' command cannot be used without a preceding 'if' command.");
    }

    bool runElse = runtime.getFlag(RUN_ELSE);
    Command::execute(args, runtime, out, interpreter);
    UNUSED(out);

    static const regex conditionRegex("[\\s]*(.*?)[\\s]*" + BLOCK_REGEX);
    smatch sm;

    if (!regex_search(args, sm, conditionRegex, regex_constants::match_continuous)) {
        throw BadCommandArgumentsException("Unknown args to command 'else': " + args);
    }

    const string condition = sm[1];
    const string code = sm[2];

    if (!runElse) {
        // A previous if condition was true, so don't execute this else
        if (!isWhitespace(condition)) {
            // If this was an else-if, allow more else conditions. Just don't run them (RUN_ELSE is not set)
            runtime.flag(ELSE_ALLOWED);
        }
        return true;
    }

    // Hack: This is just a one off, but if there are more like these, consider coming up with a more elegant solution
    stringstream ss;
    if (isWhitespace(condition)) {
        ss << code;
    } else if (condition.compare(0, strlen("if"), "if") == 0) {
        // repack
        ss << condition << " { " << code << " }";
    } else {
        throw BadCommandArgumentsException("Expected a conditional 'if' or unconditional block after the 'else' command.");
    }

    return interpreter(ss);
}
}
