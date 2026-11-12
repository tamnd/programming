// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.
// since: C++23 (std::expected)

#include "read_bytes.hpp"

#include <fstream>
#include <ios>
#include <system_error>

namespace {

// Opens path for binary reading. An ifstream opens a directory with no
// error on Linux, so check the type of the file first.
std::expected<std::ifstream, ReadError> open_file(const std::filesystem::path& path) {
    std::error_code ec;
    auto status = std::filesystem::status(path, ec);
    if (status.type() == std::filesystem::file_type::not_found) {
        return std::unexpected(ReadError::not_found);
    }
    if (std::filesystem::is_directory(status)) return std::unexpected(ReadError::not_a_file);
    std::ifstream in(path, std::ios::binary);  // binary: no change to CR LF or CTRL+Z
    if (!in) return std::unexpected(ReadError::cannot_open);
    return in;
}

// Reads up to n bytes into data and returns how many it read. read sets
// eofbit and failbit at the end of the file; gcount gives the count.
std::size_t read_into(std::ifstream& in, std::vector<std::byte>& data, std::size_t offset,
                      std::size_t n) {
    in.read(reinterpret_cast<char*>(data.data() + offset), static_cast<std::streamsize>(n));
    return static_cast<std::size_t>(in.gcount());
}

}  // namespace

std::expected<std::vector<std::byte>, ReadError> read_bytes(const std::filesystem::path& path) {
    auto in = open_file(path);
    if (!in) return std::unexpected(in.error());
    std::vector<std::byte> data;
    std::size_t size = 0;
    for (std::size_t chunk = 4096;; chunk *= 2) {
        data.resize(size + chunk);
        std::size_t got = read_into(*in, data, size, chunk);
        size += got;
        if (got < chunk) break;
    }
    if (in->bad()) return std::unexpected(ReadError::read_failed);
    data.resize(size);
    return data;
}

std::expected<std::vector<std::byte>, ReadError> read_exact(const std::filesystem::path& path,
                                                            std::size_t n) {
    auto in = open_file(path);
    if (!in) return std::unexpected(in.error());
    std::vector<std::byte> data(n);
    std::size_t got = read_into(*in, data, 0, n);
    if (in->bad()) return std::unexpected(ReadError::read_failed);
    if (got < n) return std::unexpected(ReadError::unexpected_eof);
    return data;
}
