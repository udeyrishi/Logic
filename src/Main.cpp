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
#include <lang/Interpreter.hpp>
#include <lang/Runtime.hpp>
#include <fstream>
#include <exception>

using namespace Logic;
using namespace std;

int main(int argc, char **argv) {
    Runtime *runtime = nullptr;
    Interpreter* interpreter = nullptr;
    if (argc == 1) {
        runtime = new Runtime();
        interpreter = new Interpreter(*runtime, cin, cout, true);

        interpreter->start();
        while (true) {
            try {
                interpreter->run();
                break;
            } catch (const exception &ex) {
                cerr << "ERROR: " << ex.what() << endl;
            }
        }
    } else if (argc == 2) {
        char* path = argv[1];
        ifstream infile(path);
        runtime = new Runtime();
        interpreter = new Interpreter(*runtime, infile, cout);
        interpreter->start();
        try {
            interpreter->run();
        } catch (const exception &ex) {
            cerr << "ERROR: " << ex.what() << endl;
        }
    } else {
        cerr << "Usage to start shell: " << argv[0] << endl;
        cerr << "Usage to read from file: " << argv[0] << " <path to file>" << endl;
        return -1;
    }
    delete interpreter;
    delete runtime;
    return 0;
}
