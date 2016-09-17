#include <iostream>
#include <core/TruthTable.hpp>

using namespace std;
using namespace Logic;

int main() {
    TruthTable table({"a", "b"});
    const vector<string>& vars = table.getVariables();
    for (string v : vars) {
        cout << v  << endl;
    }

    cout << table[0] << endl;
    cout << table[1] << endl;
    cout << table[2] << endl;
    cout << table[3] << endl;
    table[0] = true;
    table[3] = true;
    cout << table[0] << endl;
    cout << table[1] << endl;
    cout << table[2] << endl;
    cout << table[3] << endl;

    return 0;
}