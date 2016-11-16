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

#include <app/Modes.hpp>
#include <string>
#include <lang/Interpreter.hpp>
#include <exception>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace Logic {
int HelpMode::run() {
    cout << "Usage: " << endl;
    cout << "  " << programName << " [options]" << endl;
    cout << "    options:" << endl;
    cout << "      <no option>         : starts in interactive mode if no option is provided" << endl;
    cout << "      [filepath]          : executes the code in the text file located at <filepath>" << endl;
    cout << "      -c, --code [code]   : executes the code string passed as the command line arg itself" << endl;
    cout << "      -h, --help          : print this usage info" << endl;

    return returnCode;
}

int CodeExecutionMode::run() {
    Interpreter interpreter(runtime, dispatchTable, *codeStream, cout, config.printPrompts);
    interpreter.start();
    do {
        try {
            interpreter.run();
            break;
        } catch (const exception &ex) {
            cerr << "ERROR: " << ex.what() << endl;
        }
    } while (!config.terminateOnFailure);
    return 0;
}

CodeExecutionMode::~CodeExecutionMode() {
    if (config.ownCodeStream) {
        delete codeStream;
    }
}

unique_ptr<Mode> getMode(const int argc, const char * const *argv) {
    Mode *mode = nullptr;
    if (argc == 1) {
        // Interactive
        mode = new CodeExecutionMode({ false, false, true }, &cin);
    } else if (argc == 2) {
        string path = argv[1];
        if (path == "-c" || path == "--code") {
            // Can't use this as the file path, because this is the direct code option
            mode = new HelpMode(-1, argv[0]);
        } else if (path == "-h" || path == "--help") {
            mode = new HelpMode(0, argv[0]);
        } else {
            mode = new CodeExecutionMode({ true, true, false }, new ifstream(path));
        }
    } else if (argc == 3) {
        string option = argv[1];
        if (option == "-c" || option == "--code") {
            // Run this code
            stringstream *codeStream = new stringstream();
            (*codeStream) << argv[2];
            mode = new CodeExecutionMode({ true, true, false }, codeStream);
        } else {
            mode = new HelpMode(-1, argv[0]);
        }
    } else {
        mode = new HelpMode(-1, argv[0]);
    }

    return unique_ptr<Mode>(mode);
}
}
