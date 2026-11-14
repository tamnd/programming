// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

#include "follow_file.hpp"

#include <cerrno>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <thread>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace fs = std::filesystem;

namespace {

[[noreturn]] void fail(const std::string& what) {
    throw std::system_error(errno, std::generic_category(), what);
}

}  // namespace

Follower::Follower(fs::path path) : path_(std::move(path)) {
    fd_ = ::open(path_.c_str(), O_RDONLY);
    struct stat st;
    if (fd_ < 0 || ::fstat(fd_, &st) != 0) {
        int e = errno;
        close();
        errno = e;
        fail(path_.string());
    }
    offset_ = st.st_size;
}

Follower::~Follower() { close(); }

void Follower::close() {
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

std::vector<std::string> Follower::poll() {
    std::vector<std::string> lines;
    if (fd_ >= 0) {
        // Look at the path before the read. Then the read gets each byte that
        // was written before a rename or a delete.
        struct stat now, open_file;
        bool found = ::stat(path_.c_str(), &now) == 0;
        if (!found && errno != ENOENT) {
            fail(path_.string());
        }
        if (::fstat(fd_, &open_file) != 0) {
            fail(path_.string());
        }
        if (open_file.st_size < offset_) {
            // The file is shorter than the offset, so it was truncated.
            flush(lines);
            offset_ = 0;
        }
        read(lines);
        if (!found || now.st_dev != open_file.st_dev || now.st_ino != open_file.st_ino) {
            // The file was deleted or renamed. Its rest is read.
            flush(lines);
            close();
        }
    }
    if (fd_ < 0) {
        fd_ = ::open(path_.c_str(), O_RDONLY);
        if (fd_ < 0) {
            if (errno == ENOENT) {
                return lines;
            }
            fail(path_.string());
        }
        offset_ = 0;
        read(lines);
    }
    return lines;
}

void Follower::follow(std::chrono::milliseconds interval, const std::function<bool()>& stop,
                      const std::function<void(std::string)>& emit) {
    for (;;) {
        for (auto& line : poll()) {
            emit(std::move(line));
        }
        if (stop()) {
            return;
        }
        std::this_thread::sleep_for(interval);
    }
}

void Follower::read(std::vector<std::string>& lines) {
    char buf[65536];
    for (;;) {
        ssize_t n = ::pread(fd_, buf, sizeof buf, offset_);
        if (n < 0 && errno == EINTR) {
            continue;
        }
        if (n < 0) {
            fail(path_.string());
        }
        if (n == 0) {
            return;
        }
        offset_ += n;
        // Split the bytes at each LF. pending_ has no LF.
        std::string_view data(buf, static_cast<std::size_t>(n));
        for (auto lf = data.find('\n'); lf != std::string_view::npos; lf = data.find('\n')) {
            pending_.append(data.substr(0, lf));
            lines.push_back(std::move(pending_));
            pending_.clear();
            data.remove_prefix(lf + 1);
        }
        pending_.append(data);
    }
}

void Follower::flush(std::vector<std::string>& lines) {
    if (!pending_.empty()) {
        lines.push_back(std::move(pending_));
        pending_.clear();
    }
}

namespace {

// Writes text to the file at path, which open opens with flags.
void write_text(const fs::path& path, int flags, std::string_view text) {
    int fd = ::open(path.c_str(), O_WRONLY | flags, 0644);
    if (fd < 0) {
        fail(path.string());
    }
    while (!text.empty()) {
        ssize_t n = ::write(fd, text.data(), text.size());
        if (n < 0 && errno == EINTR) {
            continue;
        }
        if (n < 0) {
            int e = errno;
            ::close(fd);
            errno = e;
            fail(path.string());
        }
        text.remove_prefix(static_cast<std::size_t>(n));
    }
    if (::close(fd) != 0) {
        fail(path.string());
    }
}

}  // namespace

std::vector<std::vector<std::string>> follow_file(const fs::path& path, const std::string& initial,
                                                  const std::vector<std::string>& steps) {
    write_text(path, O_CREAT | O_TRUNC, initial);
    Follower follower(path);
    fs::path old = path;
    old += ".1";
    std::vector<std::vector<std::string>> polls;
    for (const auto& step : steps) {
        auto colon = step.find(':');
        std::string op = step.substr(0, colon);
        std::string text = colon == std::string::npos ? "" : step.substr(colon + 1);
        if (op == "poll") {
            polls.push_back(follower.poll());
        } else if (op == "append") {
            write_text(path, O_CREAT | O_APPEND, text);
        } else if (op == "append-old") {
            write_text(old, O_CREAT | O_APPEND, text);
        } else if (op == "truncate") {
            write_text(path, O_CREAT | O_TRUNC, text);
        } else if (op == "rotate") {
            fs::rename(path, old);
            write_text(path, O_CREAT | O_EXCL, "");
        } else if (op == "delete") {
            fs::remove(path);
        } else if (op == "create") {
            write_text(path, O_CREAT | O_EXCL, text);
        } else {
            throw std::invalid_argument("bad step: " + step);
        }
    }
    return polls;
}
