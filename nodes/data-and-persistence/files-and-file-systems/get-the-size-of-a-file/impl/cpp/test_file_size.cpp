// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

#include "file_size.hpp"

namespace fs = std::filesystem;
using namespace std::string_literals;

namespace {

const fs::path dir = "file_size_tree";
int failures = 0;

void put(const fs::path& p, const std::string& text) { std::ofstream(p, std::ios::binary) << text; }

void expect(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", what);
        ++failures;
    }
}

void fresh() {
    fs::remove_all(dir);
    fs::create_directories(dir);
}

void run_case(const char* name, const fs::path& path, const std::string& data) {
    fresh();
    put(dir / path, data);
    std::error_code ec;
    std::uintmax_t size = files::file_size(dir / path, ec);
    expect(!ec && size == data.size(), name);
}

void test_errors() {
    fresh();
    std::error_code ec;
    files::file_size(dir / "a.txt", ec);
    expect(ec == std::errc::no_such_file_or_directory, "missing");
    fs::create_directory(dir / "dir");
    files::file_size(dir / "dir", ec);
    expect(ec == std::errc::is_a_directory, "directory");
}

void test_link_sparse_open() {
    fresh();
    std::error_code ec;
    put(dir / "a.txt", "alpha beta\n");
    fs::create_symlink("a.txt", dir / "link");
    expect(files::file_size(dir / "link", ec) == 11 && !ec, "link");
    put(dir / "sparse.bin", "");
    fs::resize_file(dir / "sparse.bin", std::uintmax_t{5} << 30);
    expect(files::file_size(dir / "sparse.bin", ec) == 5368709120 && !ec, "sparse");
    std::ofstream log(dir / "app.log", std::ios::binary);
    log << "alpha\n" << std::flush;
    expect(files::file_size(dir / "app.log", ec) == 6 && !ec, "open after flush");
}

}  // namespace

int main() {
    run_case("text", "a.txt", "alpha\n");
    run_case("non-ascii-text", "a.txt", "caf\xc3\xa9\n");
    run_case("empty-file", "a.txt", "");
    run_case("binary-data", "a.bin", "\x00\r\n\xc3\xbf\x00"s);
    run_case("crlf-line-ends", "a.txt", "a\r\nb\r\n");
    run_case("name-with-spaces", "my notes.txt", "alpha\n");
    run_case("non-ascii-name", fs::path(u8"café 日本.txt"), "alpha\n");
    test_errors();
    test_link_sparse_open();
    fs::remove_all(dir);
    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    std::puts("ok");
    return 0;
}
