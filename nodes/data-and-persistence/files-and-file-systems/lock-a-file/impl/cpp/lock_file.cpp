// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

#include "lock_file.hpp"

#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>

#include <cerrno>

namespace files {

FileLock& FileLock::operator=(FileLock&& other) noexcept {
    if (this != &other) {
        release();
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

void FileLock::release() noexcept {
    if (fd_ >= 0) {
        ::flock(fd_, LOCK_UN);
        ::close(fd_);  // the close also releases the lock
        fd_ = -1;
    }
}

std::optional<FileLock> try_lock_file(const std::filesystem::path& path, LockMode mode, std::error_code& ec) {
    ec.clear();
    if (mode == LockMode::none) {
        ec = std::make_error_code(std::errc::invalid_argument);
        return std::nullopt;
    }
    // flock locks belong to the open file, so each call opens the file again.
    int fd = ::open(path.c_str(), O_RDWR);
    if (fd < 0) {
        ec = {errno, std::generic_category()};
        return std::nullopt;
    }
    int op = mode == LockMode::shared ? LOCK_SH : LOCK_EX;
    while (::flock(fd, op | LOCK_NB) != 0) {
        if (errno == EINTR) continue;
        int e = errno;
        ::close(fd);
        if (e != EWOULDBLOCK) ec = {e, std::generic_category()};
        return std::nullopt;
    }
    return FileLock(fd);
}

bool try_lock(const std::filesystem::path& path, LockMode held, LockMode want, std::error_code& ec) {
    std::optional<FileLock> first;
    if (held != LockMode::none) {
        first = try_lock_file(path, held, ec);
        if (ec) return false;
    }
    return try_lock_file(path, want, ec).has_value();
}

}  // namespace files
