// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

#pragma once

#include <filesystem>
#include <optional>
#include <system_error>

namespace files {

enum class LockMode { none, shared, exclusive };

// An flock lock on a whole file. The destructor releases the lock and closes
// the file, so the lock cannot stay after an exception.
class FileLock {
public:
    explicit FileLock(int fd) : fd_(fd) {}
    FileLock(FileLock&& other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
    FileLock& operator=(FileLock&& other) noexcept;
    FileLock(const FileLock&) = delete;
    FileLock& operator=(const FileLock&) = delete;
    ~FileLock() { release(); }

    // Releases the lock and closes the file. A second call does nothing.
    void release() noexcept;

private:
    int fd_;
};

// Asks for a shared or an exclusive lock on path, which must exist, without a
// wait. The C++ library has no file lock, so this calls POSIX open and flock.
// It returns no value and no error when another open file has a lock that
// conflicts. On an error it sets ec and returns no value.
std::optional<FileLock> try_lock_file(const std::filesystem::path& path, LockMode mode, std::error_code& ec);

// The entry of the cases: takes held, then asks for want.
bool try_lock(const std::filesystem::path& path, LockMode held, LockMode want, std::error_code& ec);

}  // namespace files
