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

#include <iostream>
#include <string>
#include <lang/Interpreter.hpp>
#include <lang/Runtime.hpp>
#include <lang/DispatchTable.hpp>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdlib.h>
#include <memory>

using namespace Logic;
using namespace std;

#define PRINT_USAGE_AND_EXIT() {        \
    printUsage(argv[0]);                \
    exit(-1);                           \
}

static void printUsage(const char *programName);
static void interpret(Interpreter &interpreter, const bool terminateOnFailure);
static DispatchTable createDispatchTable();

int main(int argc, char **argv) {
    Runtime runtime;
    DispatchTable dispatchTable = createDispatchTable();

    if (argc == 1) {
        // Interactive
        Interpreter interpreter(runtime, dispatchTable, cin, cout, true);
        interpret(interpreter, false);

    } else if (argc == 2) {
        string path = argv[1];
        if (path == "-c" || path == "--code") {
            // Can't use this as the file path, because this is the direct code option
            PRINT_USAGE_AND_EXIT();
        } else if (path == "-h" || path == "--help") {
            printUsage(argv[0]);
        } else {
            ifstream codeFileStream(path);
            Interpreter interpreter(runtime, dispatchTable, codeFileStream, cout, false);
            interpret(interpreter, true);
        }
    } else if (argc == 3) {
        string option = argv[1];
        if (option != "-c" && option != "--code") {
            PRINT_USAGE_AND_EXIT();
        }
        // Run this code
        stringstream codeStream;
        codeStream << argv[2];
        Interpreter interpreter(runtime, dispatchTable, codeStream, cout, false);
        interpret(interpreter, true);

    } else {
        PRINT_USAGE_AND_EXIT();
    }

    return 0;
}

static void interpret(Interpreter &interpreter, const bool terminateOnFailure) {
    interpreter.start();
    do {
        try {
            interpreter.run();
            break;
        } catch (const exception &ex) {
            cerr << "ERROR: " << ex.what() << endl;
        }
    } while (!terminateOnFailure);
}

static void printUsage(const char *programName) {
    cerr << "Usage: " << endl;
    cerr << "  " << programName << " [options]" << endl;
    cerr << "    options:" << endl;
    cerr << "      <no option>         : starts in interactive mode if no option is provided" << endl;
    cerr << "      [filepath]          : executes the code in the text file located at <filepath>" << endl;
    cerr << "      -c, --code [code]   : executes the code string passed as the command line arg itself" << endl;
    cerr << "      -h, --help          : print this usage info" << endl;
}

#define REGISTER_COMMAND(COMMAND, ...) \
    dispatchTable.registerCommand({ __VA_ARGS__ }, []() { return unique_ptr<Command>(new COMMAND##Command()); });

static DispatchTable createDispatchTable() {
    DispatchTable dispatchTable;
    REGISTER_COMMAND(CreateBooleanFunction, "let", "l");
    REGISTER_COMMAND(PrintBooleanFunction, "print", "p");
    REGISTER_COMMAND(CreateBooleanFunction, "delete", "d");
    REGISTER_COMMAND(PrintMinterms, "minterms", "min");
    REGISTER_COMMAND(PrintMaxterms, "maxterms", "max");
    REGISTER_COMMAND(PrintVariables, "variables", "v");
    REGISTER_COMMAND(Quit, "quit", "q");
    return dispatchTable;
}
