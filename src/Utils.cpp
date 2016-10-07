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

#include <Utils.hpp>

using namespace std;

namespace Logic {
    string trim(const string &str) {
        size_t first = str.find_first_not_of(' ');
        if (first == string::npos) {
            first = 0;
        }
        size_t last = str.find_last_not_of(' ');
        if (last == string::npos) {
            last = str.length();
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