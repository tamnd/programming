// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>

#include "truncate_file.hpp"

namespace fs = std::filesystem;
using namespace std::string_literals;

namespace {

const fs::path dir = "truncate_file_tree";
int failures = 0;

void put(const fs::path& p, const std::string& text) { std::ofstream(p, std::ios::binary) << text; }

std::string get(const fs::path& p) {
    std::ifstream in(p, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), {}};
}

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

void run_case(const char* name, const char* path, std::uintmax_t length, const std::string& before,
              const std::string& after) {
    fresh();
    put(dir / path, before);
    std::error_code ec;
    truncate_file(dir / path, length, ec);
    expect(!ec && get(dir / path) == after, name);
}

void test_errors() {
    fresh();
    std::error_code ec;
    truncate_file(dir / "a.txt", 0, ec);
    expect(ec == std::errc::no_such_file_or_directory, "missing");
    expect(!fs::exists(fs::symlink_status(dir / "a.txt")), "no file made");
    fs::create_directory(dir / "dir");
    truncate_file(dir / "dir", 0, ec);
    expect(ec == std::errc::is_a_directory, "directory");
}

void test_open() {
    fresh();
    put(dir / "app.log", "alpha beta\n");
    {
        std::ofstream log(dir / "app.log", std::ios::binary | std::ios::app);
        std::error_code ec;
        truncate_file(dir / "app.log", 0, ec);
        expect(!ec, "append truncate");
        log << "new\n";
    }
    expect(get(dir / "app.log") == "new\n", "append data");
    put(dir / "data.bin", "alpha beta\n");
    {
        std::fstream f(dir / "data.bin", std::ios::binary | std::ios::in | std::ios::out);
        f.seekp(0, std::ios::end);
        std::error_code ec;
        truncate_file(dir / "data.bin", 5, ec);
        expect(!ec, "hole truncate");
        f << 'X';
    }
    expect(get(dir / "data.bin") == "alpha\0\0\0\0\0\0X"s, "hole data");
}

}  // namespace

int main() {
    run_case("shrink", "a.txt", 5, "alpha beta\n", "alpha");
    run_case("extend", "a.txt", 8, "alpha", "alpha\0\0\0"s);
    run_case("same-length", "a.txt", 6, "alpha\n", "alpha\n");
    run_case("to-zero", "a.txt", 0, "alpha\n", "");
    run_case("empty-file", "a.txt", 3, "", "\0\0\0"s);
    run_case("name-with-spaces", "my notes.txt", 2, "alpha\n", "al");
    test_errors();
    test_open();
    fs::remove_all(dir);
    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    std::puts("ok");
    return 0;
}
