// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <system_error>

#include "file_mtime.hpp"

namespace fs = std::filesystem;
namespace chrono = std::chrono;

namespace {

const fs::path dir = "file_mtime_tree";
int failures = 0;

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

void set_mtime(const fs::path& p, std::int64_t ns) {
    chrono::sys_time<chrono::nanoseconds> t{chrono::nanoseconds{ns}};
    fs::last_write_time(p, chrono::file_clock::from_sys(t));
}

void run_case(const char* name, const char* rel, std::int64_t ns, std::int64_t want) {
    fresh();
    fs::path p = dir / rel;
    if (rel[0] == 'd') {
        fs::create_directory(p);
    } else {
        std::ofstream(p) << "alpha\n";
    }
    set_mtime(p, ns);
    std::error_code ec;
    expect(file_mtime(p, ec) == want && !ec, name);
}

void test_errors() {
    fresh();
    std::error_code ec;
    file_mtime(dir / "a.txt", ec);
    expect(ec == std::errc::no_such_file_or_directory, "missing");
}

void test_link_and_write() {
    fresh();
    std::error_code ec;
    std::ofstream(dir / "a.txt") << "alpha\n";
    set_mtime(dir / "a.txt", 1709296496000000000);
    fs::create_symlink("a.txt", dir / "link");
    expect(file_mtime(dir / "link", ec) == 1709296496 && !ec, "link");
    set_mtime(dir / "a.txt", 0);
    auto now = chrono::floor<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
    std::ofstream(dir / "a.txt", std::ios::app) << "beta\n";
    expect(file_mtime(dir / "a.txt", ec) >= now - 1 && !ec, "present time");
}

}  // namespace

int main() {
    run_case("whole-second", "a.txt", 1709296496000000000, 1709296496);
    run_case("sub-second", "a.txt", 1709296496500000000, 1709296496);
    run_case("epoch", "a.txt", 0, 0);
    run_case("before-1970", "a.txt", -86400000000000, -86400);
    run_case("before-1970-sub-second", "a.txt", -1500000000, -2);
    run_case("after-2038", "a.txt", 4102444800000000000, 4102444800);
    run_case("directory", "dir", 1709296496000000000, 1709296496);
    run_case("name-with-spaces", "my notes.txt", 1709296496000000000, 1709296496);
    test_errors();
    test_link_and_write();
    fs::remove_all(dir);
    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    std::puts("ok");
    return 0;
}
