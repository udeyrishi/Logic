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

#pragma once

#include <iostream>
#include <stdint.h>
#include <lang/Runtime.hpp>
#include <lang/DispatchTable.hpp>
#include <stdexcept>
#include <core/Utils.hpp>

using namespace std;

namespace Logic {
class Interpreter
{
public:
    Interpreter(Runtime &runtime, DispatchTable &dispatchTable, istream &in, ostream &out, const bool printPrompts)
        : runtime(runtime), dispatchTable(dispatchTable), in(in), out(out), printPrompts(printPrompts) {
    }

    void start();
    void run();

private:
    Runtime &runtime;
    DispatchTable &dispatchTable;
    istream &in;
    ostream &out;
    const bool printPrompts;

    Interpreter(const Interpreter &rhs)
        : runtime(rhs.runtime), dispatchTable(rhs.dispatchTable), in(rhs.in), out(rhs.out), printPrompts(rhs.printPrompts) {
            throw runtime_error("Copying Interpreter object not allowed.");
    }

    Interpreter &operator=(const Interpreter &rhs) {
        UNUSED(rhs);
        throw runtime_error("Copying Interpreter object not allowed.");
    }

    string nextLine(istream &in);
    void printPromptsIfNeeded();
    bool executeCode(istream &in);
};
}
