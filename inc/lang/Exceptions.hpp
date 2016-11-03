#pragma once

#include <core/Exceptions.hpp>

using namespace std;

namespace Logic {
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

class UnexpectedEOFException : public LogicException {
public:
    UnexpectedEOFException(const string &message) : LogicException(message) {
    }
};

}