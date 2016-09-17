#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>

using namespace std;

namespace Logic {
    class __TruthTableValueProxy;

    class TruthTable {
    public:
        TruthTable(const vector<string> &variables) : variables(variables), values(variables.size(), false) {
            if (variables.size() == 0) {
                throw invalid_argument("variables' size needs to be non-zero");
            }
        }

        const vector<string> &getVariables() const {
            return variables;
        }

        int size() const {
            return (int) pow(2, variables.size());
        }

        __TruthTableValueProxy operator[](const int index);

    private:
        const vector<string> variables;
        vector<bool> values;

        friend class __TruthTableValueProxy;
    };

    class __TruthTableValueProxy {
    public:
        __TruthTableValueProxy(const int index, TruthTable &table) : index(index), table(table) {
        }

        operator bool() const {
            return table.values[index];
        }

        void operator=(const bool value) {
            table.values[index] = value;
        }

    private:
        TruthTable &table;
        const int index;
    };
}
