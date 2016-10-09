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
#include <Exceptions.hpp>
#include <sstream>

using namespace std;

namespace Logic {
static constexpr char DELIMITER = ';';
static constexpr char COMMMENT_TOKEN = '#';

static void clearStringStream(stringstream &ss) {
    ss.str(string());
    ss.clear();
}

void Interpreter::printPromptsIfNeeded() {
    static const string PROMPTS = ">> ";
    if (printPrompts) {
        cout << PROMPTS;
    }
}

string Interpreter::nextLine(istream &in) {
    stringstream input;
    bool commentOngoing = false;
    char c;
    while ((c = (char) in.get())) {
        if (c == '\r') {
            // Leftover from a previous \n in Windows
            continue;
        }

        if (c == '\n') {
            commentOngoing = false;
            if (trim(input.str()).length() == 0) {
                // Nothing entered so far. Reset everything
                clearStringStream(input);
                printPromptsIfNeeded();
            } else {
                // Newline is equivalent to space in parsing
                input << ' ';
            }
            continue;
        }

        if (!commentOngoing) {
            if (c == DELIMITER) {
                string stringInput = trim(input.str());
                if (stringInput.length() == 0) {
                    clearStringStream(input);
                } else {
                    return stringInput;
                }
            } else if (c == COMMMENT_TOKEN) {
                commentOngoing = true;
            } else {
                input << c;
            }
        }
    }

    string stringInput = trim(input.str());
    if (stringInput.length() == 0) {
        // eof, with all preveious commands terminated with a ';'
        return stringInput;
    }
    throw UnexpectedEOFException("Parsed incomplete line at the end of the stream that didn't have a terminating semi-colon: " + stringInput);
}

void Interpreter::start() {
    printPromptsIfNeeded();
}

void Interpreter::run() {
    string line;
    while ((line = nextLine(in)).length() > 0) {
        uint64_t argLocation = 0;
        for (; argLocation < line.length() && !isWhitespace(line.at(argLocation)); ++argLocation);

        string commandString = line.substr(0, argLocation);
        string args = trim(line.substr(argLocation, string::npos));
        Command *command = getCommand(commandString);
        bool _continue = command->execute(args, runtime, out);
        delete command;
        if (!_continue) {
            break;
        }
    }
}
}