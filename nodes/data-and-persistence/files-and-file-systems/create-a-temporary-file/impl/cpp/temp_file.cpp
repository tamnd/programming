// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

#include "temp_file.hpp"

#include <stdlib.h>
#include <unistd.h>

#include <cerrno>
#include <fstream>
#include <iterator>
#include <vector>

namespace fs = std::filesystem;

namespace files {

namespace {

// open_temp gives the descriptor of mkstemp, or -1 with ec set.
int open_temp(const fs::path& dir, std::string_view prefix, fs::path& path, std::error_code& ec) {
    if (prefix.find('/') != std::string_view::npos) {
        ec = std::make_error_code(std::errc::invalid_argument);
        return -1;
    }
    std::string name = (dir / (std::string(prefix) + "XXXXXX")).string();
    std::vector<char> tmpl(name.begin(), name.end());
    tmpl.push_back('\0');
    int fd = ::mkstemp(tmpl.data());
    if (fd < 0) {
        ec.assign(errno, std::generic_category());
        return -1;
    }
    path = fs::path(tmpl.data());
    return fd;
}

}  // namespace

fs::path make_temp_file(const fs::path& dir, std::string_view prefix, std::error_code& ec) {
    ec.clear();
    fs::path path;
    int fd = open_temp(dir, prefix, path, ec);
    if (fd < 0) return {};
    if (::close(fd) != 0) {
        ec.assign(errno, std::generic_category());
        std::error_code ignored;
        fs::remove(path, ignored);
        return {};
    }
    return path;
}

std::string use_temp_file(const fs::path& dir, std::string_view prefix, std::string_view data,
                          std::error_code& ec) {
    fs::path path = make_temp_file(dir, prefix, ec);
    if (ec) return {};
    std::string text;
    {
        std::fstream f(path, std::ios::in | std::ios::out | std::ios::binary);
        f.write(data.data(), static_cast<std::streamsize>(data.size()));
        f.seekg(0);
        text.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
        if (!f.good() && !f.eof()) ec = std::make_error_code(std::errc::io_error);
    }
    std::error_code rm;
    fs::remove(path, rm);
    if (!ec && rm) ec = rm;
    return ec ? std::string() : text;
}

}  // namespace files
