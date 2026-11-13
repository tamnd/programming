// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <system_error>

#include "write_synced.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path dir = "sync_tree";
int failures = 0;

void expect(bool ok, const std::string& what) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", what.c_str());
        ++failures;
    }
}

void fresh() {
    fs::remove_all(dir);
    fs::create_directories(dir);
}

int count(const fs::path& d) { return static_cast<int>(std::distance(fs::directory_iterator(d), {})); }

std::string slurp(const fs::path& p) {
    std::ifstream in(p, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), {}};
}

}  // namespace

int main() {
    struct Case {
        const char* name;
        std::string text;
        std::optional<std::string> before;
    };
    const Case cases[] = {
        {"new-file", "alpha\nbeta\n", std::nullopt},
        {"replace-longer", "short\n", "a much longer old line\n"},
        {"many-lines", "one\ntwo\nthree\nfour\n", std::nullopt},
        {"empty-replace", "", "old\n"},
        {"empty-new", "", std::nullopt},
        {"no-final-newline", "last line", std::nullopt},
        {"non-ascii", "na\xc3\xafve caf\xc3\xa9\n\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\n\xf0\x9f\x99\x82\n", std::nullopt},
        {"crlf", "one\r\ntwo\r\n", std::nullopt},
    };
    const fs::path path = dir / "out.txt";
    std::error_code ec;
    for (const Case& c : cases) {
        fresh();
        if (c.before) std::ofstream(path, std::ios::binary) << *c.before;
        files::write_synced(path, c.text, ec);
        expect(!ec && slurp(path) == c.text && count(dir) == 1, c.name);
    }

    fresh();
    files::sync_directory(dir, ec);
    expect(!ec, "sync-directory");

    fresh();
    files::write_synced(dir / "missing" / "out.txt", "x\n", ec);
    expect(ec == std::errc::no_such_file_or_directory && count(dir) == 0, "missing-directory");

    fresh();
    fs::create_directory(path);
    files::write_synced(path, "x\n", ec);
    expect(ec == std::errc::is_a_directory && fs::is_directory(path), "onto-directory");

    fs::remove_all(dir);
    if (failures != 0) return 1;
    std::printf("ok\n");
    return 0;
}
