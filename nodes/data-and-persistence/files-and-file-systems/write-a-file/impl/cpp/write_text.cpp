// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

#include "write_text.hpp"

#include <cerrno>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>

#include <stdlib.h>
#include <unistd.h>

namespace fs = std::filesystem;

namespace {

// binary means no change to the line ends on any system.
void write_stream(const fs::path &path, std::string_view text, std::ios::openmode how) {
    std::ofstream out;
    out.exceptions(std::ios::failbit | std::ios::badbit);
    out.open(path, std::ios::binary | how);
    out.write(text.data(), static_cast<std::streamsize>(text.size()));
    // close flushes the buffer, so it can report a write error too.
    out.close();
}

[[noreturn]] void fail(const char *what, const fs::path &path) {
    throw fs::filesystem_error(what, path, std::error_code(errno, std::generic_category()));
}

// This part uses POSIX for the temporary file: mkstemp, write and fsync.
void replace(const fs::path &path, std::string_view text) {
    // The temporary file is in the same directory, so the rename does not
    // cross a file system.
    std::string name = path.string() + ".XXXXXX";
    std::vector<char> tmp(name.begin(), name.end());
    tmp.push_back('\0');
    int fd = mkstemp(tmp.data());
    if (fd < 0) {
        fail("mkstemp", path);
    }
    try {
        std::string_view rest = text;
        while (!rest.empty()) {
            ssize_t n = write(fd, rest.data(), rest.size());
            if (n < 0 && errno != EINTR) {
                fail("write", tmp.data());
            }
            if (n > 0) {
                rest.remove_prefix(static_cast<std::size_t>(n));
            }
        }
        if (fsync(fd) != 0) {
            fail("fsync", tmp.data());
        }
        int closed = close(fd);
        fd = -1;
        if (closed != 0) {
            fail("close", tmp.data());
        }
        fs::rename(tmp.data(), path);
    } catch (...) {
        if (fd >= 0) {
            close(fd);
        }
        std::error_code ignored;
        fs::remove(tmp.data(), ignored);
        throw;
    }
}

} // namespace

void write_text(const fs::path &path, std::string_view text, WriteMode mode) {
    switch (mode) {
    case WriteMode::Truncate:
        write_stream(path, text, std::ios::trunc);
        return;
    case WriteMode::Append:
        write_stream(path, text, std::ios::app);
        return;
    case WriteMode::Atomic:
        replace(path, text);
        return;
    }
}
