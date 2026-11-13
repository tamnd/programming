// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>

#include "make_symlink.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path dir = "make_symlink_tree";
int failures = 0;

void expect(bool ok, const std::string& what) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", what.c_str());
        ++failures;
    }
}

void fresh() {
    fs::remove_all(dir);
    fs::create_directories(dir / "sub");
    std::ofstream(dir / "a.txt", std::ios::binary) << "alpha\n";
    std::ofstream(dir / "sub" / "b.txt", std::ios::binary) << "beta\n";
}

}  // namespace

int main() {
    struct Case {
        const char *name, *target, *link;
    };
    const Case cases[] = {
        {"file-target", "a.txt", "link"},
        {"directory-target", "sub", "link"},
        {"link-in-subdirectory", "b.txt", "sub/link"},
        {"dangling-target", "missing.txt", "link"},
        {"relative-target", "a.txt", "sub/link"},
        {"parent-target", "../a.txt", "sub/link"},
        {"name-with-spaces", "my notes.txt", "my link"},
        {"non-ascii-name", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac.txt", "lien \xe6\x97\xa5\xe6\x9c\xac"},
    };
    std::error_code ec;
    for (const Case& c : cases) {
        fresh();
        fs::path link = dir / c.link;
        fs::path got = files::make_symlink(c.target, link, ec);
        expect(!ec && got == fs::path(c.target), c.name);
        expect(fs::is_symlink(link), c.name);
    }

    fresh();
    files::make_symlink("b.txt", dir / "a.txt", ec);
    expect(ec == std::errc::file_exists, "file exists");
    files::make_symlink("a.txt", dir / "sub", ec);
    expect(ec == std::errc::file_exists, "directory exists");

    fs::create_symlink("missing.txt", dir / "dangling");
    files::make_symlink("a.txt", dir / "dangling", ec);
    expect(ec == std::errc::file_exists, "dangling link exists");
    expect(fs::read_symlink(dir / "dangling") == "missing.txt", "the dangling link stays");

    files::make_symlink("a.txt", dir / "nodir" / "link", ec);
    expect(ec == std::errc::no_such_file_or_directory, "missing directory");

    files::make_symlink("../a.txt", dir / "sub" / "up", ec);
    std::ifstream in(dir / "sub" / "up", std::ios::binary);
    std::string data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    expect(!ec && data == "alpha\n", "link reads target");

    fs::remove_all(dir);
    if (failures == 0) std::printf("ok\n");
    return failures == 0 ? 0 : 1;
}
