#pragma once

#include <exception>

using namespace std;

namespace Logic {
class IllegalStateException : public exception {
public:
    IllegalStateException(const char* message) : message(message) {
    }

    virtual const char* what() const throw() {
        return message;
    }

private:
    const char* message;
};

class IllegalTruthTableException : public IllegalStateException {
public:
    IllegalTruthTableException(const char* message) : IllegalStateException(message) {
    }
};
}