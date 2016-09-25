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