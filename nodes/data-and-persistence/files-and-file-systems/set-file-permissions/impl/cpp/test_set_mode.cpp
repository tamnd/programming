// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

#include <sys/stat.h>

#include "set_mode.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path dir = "set_mode_tree";
int failures = 0;

void expect(bool ok, const std::string& what) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", what.c_str());
        ++failures;
    }
}

void fresh() {
    fs::remove_all(dir);
    fs::create_directory(dir);
}

fs::path new_file(const std::string& name) {
    fs::path p = dir / name;
    std::ofstream(p, std::ios::binary) << "alpha\n";
    fs::permissions(p, static_cast<fs::perms>(0644));
    return p;
}

unsigned mode_of(const fs::path& p) {
    return static_cast<unsigned>(fs::status(p).permissions() & fs::perms::mask);
}

}  // namespace

int main() {
    struct Case {
        const char *name, *rel, *mode;
        unsigned result;
    };
    const Case cases[] = {
        {"owner-read-write-group-read", "a.txt", "640", 0640},
        {"owner-only", "a.txt", "600", 0600},
        {"executable", "a.txt", "755", 0755},
        {"same-mode", "a.txt", "644", 0644},
        {"no-permissions", "a.txt", "000", 0},
        {"leading-zero", "a.txt", "0640", 0640},
        {"set-user-id", "a.txt", "4755", 04755},
        {"name-with-spaces", "my notes.txt", "640", 0640},
        {"non-ascii-name", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac.txt", "640", 0640},
    };
    std::error_code ec;
    for (const Case& c : cases) {
        fresh();
        fs::path p = new_file(c.rel);
        unsigned got = files::set_mode(p, c.mode, ec);
        expect(!ec && got == c.result, c.name);
        expect(mode_of(p) == c.result, c.name);
    }

    fresh();
    fs::path a = new_file("a.txt");
    for (const char* m : {"648", "64", "10640", "u+x", "", "640\n", "+640"}) {
        files::set_mode(a, m, ec);
        expect(ec == std::errc::invalid_argument, std::string("invalid ") + m);
    }
    expect(mode_of(a) == 0644, "invalid modes change nothing");

    files::set_mode(dir / "missing.txt", "640", ec);
    expect(ec == std::errc::no_such_file_or_directory, "missing");

    fresh();
    a = new_file("a.txt");
    fs::create_symlink("a.txt", dir / "link");
    expect(files::set_mode(dir / "link", "600", ec) == 0600 && !ec, "link");
    expect(mode_of(a) == 0600, "link changes the target");

    fresh();
    fs::create_directory(dir / "dir");
    expect(files::set_mode(dir / "dir", "700", ec) == 0700 && !ec, "directory");

    fresh();
    a = new_file("a.txt");
    mode_t old = umask(077);
    expect(files::set_mode(a, "666", ec) == 0666 && !ec, "umask does not apply");
    umask(old);

    fs::remove_all(dir);
    if (failures == 0) std::printf("ok\n");
    return failures == 0 ? 0 : 1;
}
