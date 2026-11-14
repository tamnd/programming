// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

#pragma once

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>

#include <sys/types.h>

// Reads the lines that are added to a file, one poll at a time, as tail -F
// does. The follower keeps the file open, so it can read the rest of the file
// after a rename or a delete. std::filesystem cannot get the identity of an
// open file, so the follower uses the POSIX calls open, fstat and pread. Each
// error is a std::system_error.
class Follower {
public:
    // Opens the file at path and starts at its end. The error code is
    // std::errc::no_such_file_or_directory if no file is at path.
    explicit Follower(std::filesystem::path path);
    Follower(const Follower&) = delete;
    Follower& operator=(const Follower&) = delete;
    ~Follower();

    // Returns the complete lines that were added since the last poll.
    std::vector<std::string> poll();

    // Calls emit with each new line. Polls the file each interval, and
    // returns when stop returns true.
    void follow(std::chrono::milliseconds interval, const std::function<bool()>& stop,
                const std::function<void(std::string)>& emit);

private:
    void read(std::vector<std::string>& lines);
    void flush(std::vector<std::string>& lines);
    void close();

    std::filesystem::path path_;
    int fd_ = -1;  // -1 if no file is open
    off_t offset_ = 0;
    std::string pending_;  // the bytes after the last LF
};

// Writes initial to path, makes a follower, does the steps and returns the
// lines of each poll.
std::vector<std::vector<std::string>> follow_file(const std::filesystem::path& path, const std::string& initial,
                                                  const std::vector<std::string>& steps);
