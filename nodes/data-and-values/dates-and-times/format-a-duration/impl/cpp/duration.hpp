// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

#ifndef FORMAT_DURATION_HPP
#define FORMAT_DURATION_HPP

#include <chrono>
#include <format>
#include <string>

// Returns the duration as HH:MM:SS. For a duration, %T does not wrap the
// hours at 24, and it writes a minus sign for a negative duration.
inline std::string format_duration(long long seconds) {
    return std::format("{:%T}", std::chrono::seconds{seconds});
}

#endif
