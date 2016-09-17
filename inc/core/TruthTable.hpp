#pragma once

#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <exception>
#include <stdint.h>

using namespace std;

namespace Logic {
    class __TruthTableValueProxy;

    // TODO: max uint (64 bits) => max size of table == 2**64.
    // TODO: Thus, max number of variables == 64. num variables can be stored in a uint8_t
    class TruthTable {
    public:
        TruthTable(const vector<string> &variables);

        const vector<string> &getVariables() const {
            return variables;
        }

        uint64_t size() const {
            return (uint64_t) pow(2, variables.size());
        }

        __TruthTableValueProxy operator[](const uint64_t index);

        bool operator[](const uint64_t index) const;

    private:
        const vector<string> variables;
        vector<bool> values;

        void validateIndex(const uint64_t index) const;

        friend class __TruthTableValueProxy;
    };

    class __TruthTableValueProxy {
    public:
        __TruthTableValueProxy(const uint64_t index, TruthTable &table) : index(index), table(table) {
        }

        operator bool() const {
            return table.values[index];
        }

        void operator=(const bool value) {
            table.values[index] = value;
        }

    private:
        const uint64_t index;
        TruthTable &table;
    };

    class TruthTableBuilder {
    public:
        void set(uint64_t lineIndex, const bool b);

        void setVariables(const vector<string> &variables) {
            this->variables = variables;
        }

        TruthTable build() const;

    private:
        vector<bool> values;
        vector<string> variables;
    };

    class IllegalTruthTableException : public exception {
    public:
        IllegalTruthTableException(const char* message) : message(message) {
        }

        virtual const char* what() const throw() {
            return message;
        }

    private:
        const char* message;
    };
}
