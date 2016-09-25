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
}