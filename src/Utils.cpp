#include <Utils.hpp>
#include <regex>
#include <stdexcept>

using namespace std;

namespace Logic {
    string trim(const string &str) {
        const static regex whitespaceRemover("\\s*(.*)\\s*");
        smatch sm;
        if (regex_match(str, sm, whitespaceRemover, regex_constants::match_continuous)) {
            return sm[1];
        } else {
            throw runtime_error("Unknown error occurred when trying to trim the string: " + str);
        }
    }
}