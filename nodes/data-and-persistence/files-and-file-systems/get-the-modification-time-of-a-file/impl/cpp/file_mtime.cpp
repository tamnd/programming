// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

#include "file_mtime.hpp"

#include <chrono>

namespace fs = std::filesystem;

std::int64_t file_mtime(const fs::path& path, std::error_code& ec) {
    fs::file_time_type t = fs::last_write_time(path, ec);
    if (ec) return 0;
    // The epoch of file_clock is not given by the standard, so the code
    // converts the time to system_clock, which counts from 1970. floor, not
    // duration_cast, gives the second before a time before 1970.
    auto sys = std::chrono::file_clock::to_sys(t);
    return std::chrono::floor<std::chrono::seconds>(sys.time_since_epoch()).count();
}
