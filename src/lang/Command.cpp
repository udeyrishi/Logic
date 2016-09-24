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
    if (regex_search(args, sm, CREATE_ARGS_REGEX, regex_constants::match_continuous)) {
        string variableName = sm[1];
        BooleanFunctionParser functionParser;
        runtime.save(variableName, functionParser.parse(sm[2]));
    } else {
        throw BadCommandArgumentsException("Unknown args to command 'let': " + args);
    }
}

Command *getCommand(const string &command) {
    if (command == CREATE_BOOLEAN_FUNCTION_COMMAND) {
        return new CreateBooleanFunctionCommand();
    }

    throw UnknownCommandException("Unknown command: " + command);
}
}
