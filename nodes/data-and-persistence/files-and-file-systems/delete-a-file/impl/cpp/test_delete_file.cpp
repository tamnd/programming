// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>

#include <unistd.h>

#include "delete_file.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path dir = "delete_file_tree";
int failures = 0;

void touch(const fs::path& p, const std::string& text) {
    std::ofstream(p, std::ios::binary) << text;
}

bool lexists(const fs::path& p) {
    std::error_code ec;
    return fs::exists(fs::symlink_status(p, ec));
}

void check(const char* name, const fs::path& p, bool want) {
    auto got = delete_file(p);
    if (!got || *got != want || lexists(p)) {
        std::fprintf(stderr, "%s: %s\n", name, got ? (*got ? "true" : "false") : got.error().message().c_str());
        ++failures;
    }
}

void expect(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", what);
        ++failures;
    }
}

}  // namespace

int main() {
    fs::remove_all(dir);
    fs::create_directory(dir);

    // The cases of cases/basic.jsonl and cases/edge.jsonl.
    touch(dir / "a.txt", "alpha\n");
    check("delete", dir / "a.txt", true);
    check("missing", dir / "a.txt", false);
    touch(dir / "a.txt", "");
    check("empty-file", dir / "a.txt", true);
    check("missing-parent", dir / "no-such-dir" / "a.txt", false);
    touch(dir / "my notes.txt", "alpha\n");
    check("name-with-spaces", dir / "my notes.txt", true);
    touch(dir / "café 日本.txt", "alpha\n");
    check("non-ascii-name", dir / "café 日本.txt", true);

    touch(dir / "a.txt", "alpha\n");
    check("through-a-file", dir / "a.txt" / "b.txt", false);

    fs::create_directory(dir / "dir");
    auto d = delete_file(dir / "dir");
    expect(!d && d.error() == std::errc::is_a_directory && fs::is_directory(dir / "dir"),
           "directory: no is_a_directory error");

    fs::create_symlink("a.txt", dir / "to-file");
    fs::create_directory_symlink("dir", dir / "to-dir");
    fs::create_symlink("missing.txt", dir / "broken");
    check("to-file", dir / "to-file", true);
    check("to-dir", dir / "to-dir", true);
    check("broken", dir / "broken", true);
    expect(fs::is_regular_file(dir / "a.txt") && fs::is_directory(dir / "dir"), "links: a target is gone");

    fs::permissions(dir / "a.txt", fs::perms::owner_read | fs::perms::group_read | fs::perms::others_read);
    check("read-only", dir / "a.txt", true);

    if (geteuid() != 0) {
        fs::create_directory(dir / "locked");
        touch(dir / "locked" / "a.txt", "alpha\n");
        fs::permissions(dir / "locked", fs::perms::owner_read | fs::perms::owner_exec);
        auto r = delete_file(dir / "locked" / "a.txt");
        expect(!r && r.error() == std::errc::permission_denied && fs::exists(dir / "locked" / "a.txt"),
               "locked: no permission error");
        fs::permissions(dir / "locked", fs::perms::owner_all);
    }

    touch(dir / "open.txt", "alpha\n");
    {
        std::ifstream in(dir / "open.txt", std::ios::binary);
        check("open", dir / "open.txt", true);
        std::string data{std::istreambuf_iterator<char>(in), {}};
        expect(data == "alpha\n", "open: cannot read the data");
    }

    fs::remove_all(dir);
    if (failures == 0) std::puts("ok");
    return failures == 0 ? 0 : 1;
}
