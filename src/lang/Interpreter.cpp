#include <lang/Interpreter.hpp>
#include <string>
#include <lang/Command.hpp>
#include <Utils.hpp>

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

        // There could be some whitespace after the DELIMITER on the previous line
        bool nonSpaceCharFound = false;
        uint64_t start = 0;
        for (i = 0; i < line.length(); ++i) {
            if (isWhitespace(lineChars[i]) && nonSpaceCharFound) {
                break;
            } else if (!isWhitespace(lineChars[i]) && !nonSpaceCharFound) {
                nonSpaceCharFound = true;
                start = i;
            }
        }
        string commandString = line.substr(start, i - start);
        while (i < line.length() && isWhitespace(lineChars[i])) {
            ++i;
        }
        // args can be empty
        string args = trim(line.substr(i, string::npos));
        Command *command = getCommand(commandString);
        command->execute(args, runtime, out);
        delete command;
    }
}