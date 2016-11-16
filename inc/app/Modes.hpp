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

#include <memory>
#include <lang/Runtime.hpp>
#include <lang/DispatchTable.hpp>
#include <istream>

using namespace std;

namespace Logic {

class Mode {
public:
    virtual int run() = 0;
    virtual ~Mode() {
    }
};

class HelpMode : public Mode {
public:
    HelpMode(const int returnCode, const string &&programName) : returnCode(returnCode), programName(programName) {
    }
    virtual int run() override;

private:
    const int returnCode;
    const string programName;
};

struct CodeExecutionModeConfig {
    const bool terminateOnFailure;
    const bool ownCodeStream;
    const bool printPrompts;
};

class CodeExecutionMode : public Mode {
public:
    CodeExecutionMode(const CodeExecutionModeConfig &&config, istream *codeStream)
        : dispatchTable(createDispatchTableWithAllCommands()), config(config), codeStream(codeStream) {
    }

    virtual int run() override;
    virtual ~CodeExecutionMode() override;

private:
    Runtime runtime;
    DispatchTable dispatchTable;
    const CodeExecutionModeConfig config;
    istream *codeStream;
};

unique_ptr<Mode> getMode(const int argc, const char * const *argv);
}
