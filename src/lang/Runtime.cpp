#include <lang/Runtime.hpp>
#include <Exceptions.hpp>

using namespace std;

namespace Logic {
void Runtime::save(const string &variableName, const BooleanFunction &function) {
    const auto &found = workspace.find(variableName);
    if (found == workspace.end()) {
        workspace.insert(make_pair(variableName, function));
    } else {
        found->second = function;
    }
}

BooleanFunction &Runtime::get(const string &variableName) {
    const auto &found = workspace.find(variableName);
    if (found == workspace.end()) {
        throw BooleanFunctionNotFoundException("Boolean Function not found in the current workspace: " + variableName);
    }
    return found->second;
}

bool Runtime::contains(const string &variableName) const {
    return workspace.find(variableName) != workspace.end();
}
}