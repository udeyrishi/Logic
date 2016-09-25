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
