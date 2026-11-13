// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <system_error>
#include <vector>

namespace files {

// The cases set the mtime to base_mtime + step, in seconds.
inline constexpr std::int64_t base_mtime = 1'700'000'000;

// The size and the mtime of a file.
struct Signature {
    std::uintmax_t size;
    std::filesystem::file_time_type mtime;
    bool operator==(const Signature&) const = default;
};

// Remembers the last signature of a file and tells what changed since then.
class FileWatcher {
public:
    // Takes the first signature; ec is set for a directory or an error of stat.
    FileWatcher(std::filesystem::path path, std::error_code& ec);

    // created, deleted, modified or none, for the time since the last poll.
    std::string poll(std::error_code& ec);

private:
    std::filesystem::path path_;
    std::optional<Signature> last_;
};

// Calls on_change once for each burst of changes, after the file is quiet,
// until stop returns true.
void watch(const std::filesystem::path& path, const std::function<void(const std::filesystem::path&)>& on_change,
           std::chrono::milliseconds interval, std::chrono::milliseconds quiet, const std::function<bool()>& stop,
           std::error_code& ec);

// The entry of the cases: applies each step, polls after each, and returns the results.
std::vector<std::string> watch_changes(const std::filesystem::path& path, const std::vector<std::string>& steps,
                                       std::error_code& ec);

}  // namespace files
