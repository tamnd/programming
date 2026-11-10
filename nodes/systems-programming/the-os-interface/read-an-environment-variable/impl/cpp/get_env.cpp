// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

#include "get_env.hpp"

#include <cstdlib>

std::string get_env(const std::string& name, std::string_view default_value) {
    // std::getenv returns a null pointer when the variable is not set. The
    // string belongs to the environment, so the function copies it at once.
    if (const char* value = std::getenv(name.c_str())) {
        return value;
    }
    return std::string(default_value);
}
