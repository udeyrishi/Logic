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

using namespace std;
// For explicitly removing -Wunused-parameter
#define UNUSED(expr) do { (void)(expr); } while (0)

namespace Logic {
    bool isWhitespace(char c);
    bool isWhitespace(const string &str);
    string trim(const string &str);
    vector<string> split(string str, const char delim);
    vector<string> split(string str, const string &delim);
    string join(const vector<string> &vec, const string &delimiter);

    template <typename TCollection, typename TValue>
    bool contains(TCollection collection, TValue item) {
        for (TValue value : collection) {
            if (value == item) {
                return true;
            }
        }
        return false;
    }
}
