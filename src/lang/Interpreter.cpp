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

#include <lang/Interpreter.hpp>
#include <string>
#include <lang/Command.hpp>
#include <Utils.hpp>

using namespace std;
using namespace Logic;

static const char DELIMITER = ';';
static const string PROMPTS = ">> ";

static bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void Interpreter::run() {
    string line;
    if (printPrompts) {
        cout << PROMPTS;
    }

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
        if (printPrompts) {
            cout << PROMPTS;
        }
    }
}