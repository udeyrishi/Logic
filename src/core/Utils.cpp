/**
 Copyright 2016 Udey Rishi

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include <core/Utils.hpp>
#include <core/Exceptions.hpp>

using namespace std;

namespace Logic {
    bool isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    bool isWhitespace(const string &str) {
        for (const char c : str) {
            if (!isWhitespace(c)) {
                return false;
            }
        }
        return true;
    }

    string trim(const string &str) {
        size_t first = string::npos;
        for (size_t i = 0; i < str.length(); ++i) {
            if (!isWhitespace(str.at(i))) {
                first = i;
                break;
            }
        }
        if (first == string::npos) {
            return "";
        }

        size_t last = string::npos;
        for (size_t i = str.length(); i > 0; --i) {
            if (!isWhitespace(str.at(i-1))) {
                last = i-1;
                break;
            }
        }
        if (last == string::npos) {
            throw IllegalStateException("Dev note: Iterated through the entire string backwards looking for a non-whitespace char. Not possible.");
        }
        return str.substr(first, (last-first+1));
    }

    vector<string> split(string str, const char delim) {
        return split(str, string(1, delim));
    }

    vector<string> split(string str, const string &delim) {
        vector<string> parts;
        size_t index;
        while ((index = str.find(delim)) != string::npos) {
            parts.push_back(str.substr(0, index));
            str = str.substr(index + 1, string::npos);
        }

        if (!str.empty()) {
            parts.push_back(str);
        }
        return parts;
    }
}
