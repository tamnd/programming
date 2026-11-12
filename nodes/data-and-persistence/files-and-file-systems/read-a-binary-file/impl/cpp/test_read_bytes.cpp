// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <map>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "read_bytes.hpp"

namespace {

const std::filesystem::path input = "read_bytes_input.bin";

using Bytes = std::vector<std::byte>;

Bytes hex(std::string_view s) {
    Bytes b;
    for (std::size_t i = 0; i + 1 < s.size(); i += 2) {
        b.push_back(static_cast<std::byte>(std::stoi(std::string(s.substr(i, 2)), nullptr, 16)));
    }
    return b;
}

Bytes pattern(std::size_t n) {
    Bytes b(n);
    for (std::size_t i = 0; i < n; ++i) b[i] = static_cast<std::byte>(i);
    return b;
}

void write_file(const Bytes& data) {
    std::ofstream out(input, std::ios::binary);
    out.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
}

int failures = 0;

void check(std::string_view name, bool ok) {
    if (!ok) {
        std::fprintf(stderr, "case %.*s failed\n", static_cast<int>(name.size()), name.data());
        ++failures;
    }
}

}  // namespace

int main() {
    const std::map<std::string, Bytes> data = {
        {"signature", hex("89504e470d0a1a0a0000000d49484452")},
        {"bytes", hex("0001027f80feff")},
        {"empty", {}},
        {"all-bytes", pattern(256)},
        {"zeros", Bytes(8)},
        {"crlf", hex("610d0a620d0a")},
        {"ctrl-z", hex("611a62")},
        {"not-utf8", hex("c328fffe80")},
        {"large", pattern(1 << 20)},
    };
    for (const auto& [name, bytes] : data) {
        write_file(bytes);
        auto got = read_bytes(input);
        check(name, got && *got == bytes);
    }

    const std::vector<std::tuple<std::string, std::string, std::size_t, Bytes>> exact = {
        {"signature-only", "signature", 8, hex("89504e470d0a1a0a")},
        {"first-four", "all-bytes", 4, hex("00010203")},
        {"whole-file", "zeros", 8, Bytes(8)},
        {"zero-count", "empty", 0, {}},
        {"across-ctrl-z", "ctrl-z", 3, hex("611a62")},
        {"large", "large", 1 << 20, pattern(1 << 20)},
    };
    for (const auto& [name, file, n, want] : exact) {
        write_file(data.at(file));
        auto got = read_exact(input, n);
        check(name, got && *got == want);
    }

    for (const auto& [name, file, n] : {std::tuple{"one-past-end", "signature", 17},
                                        std::tuple{"empty-file", "empty", 1}}) {
        write_file(data.at(file));
        auto got = read_exact(input, n);
        check(name, !got && got.error() == ReadError::unexpected_eof);
    }

    std::filesystem::remove(input);
    auto missing = read_bytes("no_such_file.bin");
    check("missing", !missing && missing.error() == ReadError::not_found);
    auto missing_exact = read_exact("no_such_file.bin", 1);
    check("exact-missing", !missing_exact && missing_exact.error() == ReadError::not_found);
    auto dir = read_bytes(".");
    check("directory", !dir && dir.error() == ReadError::not_a_file);
    auto dir_exact = read_exact(".", 1);
    check("exact-directory", !dir_exact && dir_exact.error() == ReadError::not_a_file);

    if (failures > 0) return 1;
    std::puts("all tests passed");
    return 0;
}
