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

#pragma once

#include <string>
#include <vector>
#include <sstream>

using namespace std;
// For explicitly removing -Wunused-parameter
#define UNUSED(expr) do { (void)(expr); } while (0)

namespace Logic {
    bool isWhitespace(char c);
    bool isWhitespace(const string &str);
    string trim(const string &str);
    vector<string> split(string str, const char delim);
    vector<string> split(string str, const string &delim);

    template <typename T>
    string join(const vector<T> &vec, const string &delimiter) {
        stringstream joined;
        for (uint64_t i = 0; i < vec.size(); ++i) {
            joined << vec[i];
            if (i < vec.size() - 1) {
                joined << delimiter;
            }
        }
        return joined.str();
    }

    template <typename TCollection, typename TValue>
    bool contains(const TCollection &collection, const TValue &item) {
        for (const TValue &value : collection) {
            if (value == item) {
                return true;
            }
        }
        return false;
    }
}
