// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace greetings {

// Starts one thread per name; thread i builds "hello from " + names[i].
// Throws std::system_error if a thread cannot be started.
std::vector<std::string> thread_greetings(const std::vector<std::string_view>& names);

}  // namespace greetings
