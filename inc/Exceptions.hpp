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

#include <exception>

using namespace std;

namespace Logic {

class LogicException : public exception {
public:
    LogicException(const string &message) : message(message) {
    }

    virtual const char* what() const throw() {
        return message.c_str();
    }

private:
    const string message;
};

class IllegalStateException : public LogicException {
public:
    IllegalStateException(const string &message) : LogicException(message) {
    }
};

class IllegalTruthTableException : public IllegalStateException {
public:
    IllegalTruthTableException(const string &message) : IllegalStateException(message) {
    }
};

class BadBooleanFunctionException : public LogicException {
public:
    BadBooleanFunctionException(const string &message) : LogicException(message) {
    }
};

class UnknownTokenException : public BadBooleanFunctionException {
public:
    UnknownTokenException(const string &message) : BadBooleanFunctionException(message) {
    }
};

class UnknownCommandException : public LogicException {
public:
    UnknownCommandException(const string &message) : LogicException(message) {
    }
};

class BadCommandArgumentsException : public LogicException {
public:
    BadCommandArgumentsException(const string &message) : LogicException(message) {
    }
};

class BooleanFunctionNotFoundException : public LogicException {
public:
    BooleanFunctionNotFoundException(const string &message) : LogicException(message) {
    }
};

class UnexpectedEOFException : public LogicException {
public:
    UnexpectedEOFException(const string &message) : LogicException(message) {
    }
};
}