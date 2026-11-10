// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

#pragma once

#include <string>
#include <string_view>

// Returns a copy of the value of the environment variable name, or
// default_value when it is not set. An empty value is a value, so it does not
// give default_value.
std::string get_env(const std::string& name, std::string_view default_value);
