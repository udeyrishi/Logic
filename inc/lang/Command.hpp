#pragma once

#include <string>
#include <lang/Runtime.hpp>
#include <iostream>

using namespace std;

namespace Logic {
class Command {
public:
    virtual void execute(const string &args, Runtime &runtime, ostream &out) = 0;
    virtual ~Command() {
    }
};

static const string CREATE_BOOLEAN_FUNCTION_COMMAND("let");
class CreateBooleanFunctionCommand : public Command  {
public:
    virtual void execute(const string &args, Runtime &runtime, ostream &out);
};

static const string PRINT_BOOLEAN_FUNCTION_COMMAND("print");
class PrintBooleanFunctionCommand : public Command {
public:
    virtual void execute(const string &functionName, Runtime &runtime, ostream &out);
};

Command *getCommand(const string &command);
}