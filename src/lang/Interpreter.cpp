#include <lang/Interpreter.hpp>
#include <string>
#include <lang/Command.hpp>

using namespace std;
using namespace Logic;

static const char DELIMITER = ';';

static bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void Interpreter::run() {
    string line;
    while (getline(in, line, DELIMITER)) {
        const char *lineChars = line.c_str();
        uint64_t i;
        for (i = 0; i < line.length(); ++i) {
            if (isWhitespace(lineChars[i])) {
                break;
            }
        }
        string commandString = line.substr(0, i);
        while (i < line.length() && isWhitespace(lineChars[i])) {
            ++i;
        }
        // Can be empty
        string args = line.substr(i, string::npos);
        Command *command = getCommand(commandString);
        command->execute(args, runtime, out);
        delete command;
    }
}