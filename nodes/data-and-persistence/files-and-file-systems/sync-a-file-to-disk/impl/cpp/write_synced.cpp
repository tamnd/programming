// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

#include "write_synced.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>

namespace fs = std::filesystem;

namespace files {

namespace {

std::error_code last_error() { return {errno, std::generic_category()}; }

// fail sets ec from errno, then closes fd.
void fail(int fd, std::error_code& ec) {
    ec = last_error();
    ::close(fd);
}

}  // namespace

void sync_directory(const fs::path& dir, std::error_code& ec) {
    ec.clear();
    int fd = ::open(dir.c_str(), O_RDONLY | O_DIRECTORY);
    if (fd < 0) {
        ec = last_error();
        return;
    }
    if (::fsync(fd) != 0) {
        fail(fd, ec);
        return;
    }
    if (::close(fd) != 0) ec = last_error();
}

void write_synced(const fs::path& path, std::string_view text, std::error_code& ec) {
    ec.clear();
    int fd = ::open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        ec = last_error();
        return;
    }
    // write can write fewer bytes than asked, so it runs in a loop.
    for (std::size_t done = 0; done < text.size();) {
        ssize_t n = ::write(fd, text.data() + done, text.size() - done);
        if (n < 0) {
            if (errno == EINTR) continue;
            fail(fd, ec);
            return;
        }
        done += static_cast<std::size_t>(n);
    }
    if (::fsync(fd) != 0) {
        fail(fd, ec);
        return;
    }
    if (::close(fd) != 0) {
        ec = last_error();
        return;
    }
    fs::path abs = fs::absolute(path, ec);
    if (ec) return;
    sync_directory(abs.parent_path(), ec);
}

}  // namespace files
