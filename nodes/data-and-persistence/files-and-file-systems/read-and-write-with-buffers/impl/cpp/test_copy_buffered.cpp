// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

#include "copy_buffered.hpp"

namespace {

const std::filesystem::path src = "copy_buffered_in.bin";
const std::filesystem::path dst = "copy_buffered_out.bin";

void write_file(const std::filesystem::path& path, const std::string& data) {
    std::ofstream f(path, std::ios::binary | std::ios::trunc);
    f.write(data.data(), static_cast<std::streamsize>(data.size()));
    if (!f.flush()) std::abort();
}

std::string read_file(const std::filesystem::path& path) {
    std::ifstream f(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
}

std::string pattern(std::size_t n) {
    std::string s(n, '\0');
    for (std::size_t i = 0; i < n; i++) s[i] = static_cast<char>(i & 0xff);
    return s;
}

int failures = 0;

void fail(std::string_view name, std::string_view what) {
    std::fprintf(stderr, "case %.*s: %.*s\n", static_cast<int>(name.size()), name.data(),
                 static_cast<int>(what.size()), what.data());
    failures++;
}

void check(std::string_view name, const std::string& data, std::streamsize size) {
    write_file(src, data);
    auto got = copy_buffered(src, dst, size);
    if (!got || *got != data.size()) return fail(name, "wrong count or an error");
    if (read_file(dst) != data) fail(name, "dst is not the same as src");
}

void check_error(std::string_view name, const std::filesystem::path& from,
                 const std::filesystem::path& to, std::streamsize size, CopyError want) {
    auto got = copy_buffered(from, to, size);
    if (got || got.error() != want) fail(name, "wrong result, want an error");
}

}  // namespace

int main() {
    const std::string text = "alpha\nbeta\ngamma\n";
    const std::string large = pattern(1 << 20);
    check("text", text, 4096);
    check("several-buffers", text, 4);
    check("empty", "", 8);
    check("smaller-than-buffer", "12345", 8);
    check("exact-buffer", "12345678", 8);
    check("one-past-buffer", "123456789", 8);
    check("size-one", text, 1);
    check("binary", pattern(256), 64);
    check("large", large, 4096);
    check("large-odd-buffer", large, 4093);

    write_file(dst, "old and longer");
    check("replaces-a-longer-file", "new", 8);

    std::filesystem::remove(dst);
    check_error("missing-src", "copy_buffered_missing.bin", dst, 8, CopyError::not_found);
    if (std::filesystem::exists(dst)) fail("missing-src", "dst was made");
    write_file(src, text);
    check_error("missing-dst-dir", src, "copy_buffered_no_such_dir/out.bin", 8, CopyError::not_found);
    check_error("zero-size", src, dst, 0, CopyError::invalid_size);
    check_error("negative-size", src, dst, -1, CopyError::invalid_size);

    if (std::filesystem::exists("/dev/full")) {
        // 17 bytes stay in the buffer, so the error comes only from close.
        check_error("full-device-small", src, "/dev/full", 4096, CopyError::write_failed);
        write_file(src, large);
        check_error("full-device-large", src, "/dev/full", 4096, CopyError::write_failed);
    }
    std::filesystem::remove(src);
    std::filesystem::remove(dst);
    if (failures > 0) return 1;
    std::puts("ok");
}
