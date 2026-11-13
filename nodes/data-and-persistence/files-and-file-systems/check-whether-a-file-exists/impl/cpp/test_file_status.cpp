// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

#include <unistd.h>

#include "file_status.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path dir = "file_status_tree";
int failures = 0;

void check(const std::string& path, std::string_view want) {
    auto got = file_status(path);
    if (!got || *got != want) {
        std::fprintf(stderr, "file_status(%s) is wrong\n", path.c_str());
        failures++;
    }
}

}  // namespace

int main() {
    fs::remove_all(dir);
    fs::create_directories(dir / "dir");
    std::ofstream(dir / "a.txt") << "alpha\n";
    std::ofstream(dir / "empty.txt").close();
    const std::string d = dir.string() + "/";
    check(d + "a.txt", "file");
    check(d + "empty.txt", "file");
    check(d + "dir", "directory");
    check(d + "dir/", "directory");
    check(d + "missing.txt", "missing");
    check(d + "no-such-dir/a.txt", "missing");
    check(d + "a.txt/b.txt", "missing");

    fs::create_symlink("a.txt", dir / "to-file");
    fs::create_directory_symlink("dir", dir / "to-dir");
    fs::create_symlink("missing.txt", dir / "to-nothing");
    check(d + "to-file", "file");
    check(d + "to-dir", "directory");
    check(d + "to-nothing", "broken-link");

    if (fs::exists("/dev/null")) check("/dev/null", "other");
    if (std::system(("mkfifo " + d + "pipe").c_str()) != 0) return 2;
    check(d + "pipe", "other");

    if (geteuid() != 0) {
        fs::create_directory(dir / "locked");
        std::ofstream(dir / "locked" / "a.txt") << "alpha\n";
        fs::permissions(dir / "locked", fs::perms::none);
        auto got = file_status(dir / "locked" / "a.txt");
        if (got || got.error() != std::errc::permission_denied) {
            std::fputs("locked: want permission_denied\n", stderr);
            failures++;
        }
        fs::permissions(dir / "locked", fs::perms::owner_all);
    }
    fs::remove_all(dir);
    if (failures > 0) return 1;
    std::puts("ok");
}
