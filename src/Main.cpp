#include <iostream>
#include <lang/Interpreter.hpp>
#include <lang/Runtime.hpp>

using namespace Logic;

int main() {
    Runtime *runtime = new Runtime();
    Interpreter interpreter(*runtime, cin, cout);
    interpreter.run();
    delete runtime;
    return 0;
}
