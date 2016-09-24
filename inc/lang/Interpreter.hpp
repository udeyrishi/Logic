#pragma once

#include <iostream>
#include <stdint.h>
#include <lang/Runtime.hpp>
#include <stdexcept>
#include <Utils.hpp>

using namespace std;

namespace Logic {
class Interpreter
{
public:
    Interpreter(Runtime &runtime, istream &in, ostream &out)
        : runtime(runtime), in(in), out(out) {
    }

    void run();

private:
    Runtime &runtime;
    istream &in;
    ostream &out;

    Interpreter(const Interpreter &rhs)
        : runtime(rhs.runtime), in(rhs.in), out(rhs.out) {
            throw runtime_error("Copying Interpreter object not allowed.");
    }

    Interpreter &operator=(const Interpreter &rhs) {
        UNUSED(rhs);
        throw runtime_error("Copying Interpreter object not allowed.");
    }
};
}