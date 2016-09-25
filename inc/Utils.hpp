#pragma once

#include <string>

using namespace std;
// For explicitly removing -Wunused-parameter
#define UNUSED(expr) do { (void)(expr); } while (0)

namespace Logic {
    string trim(const string &str);
}