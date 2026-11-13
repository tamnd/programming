// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

#include <cstdio>
#include <filesystem>
#include <iterator>
#include <string>
#include <system_error>

#include "temp_file.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path dir = "temp_file_tree";
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
}

long count(const fs::path& d) {
    return std::distance(fs::directory_iterator(d), fs::directory_iterator());
}

}  // namespace

int main() {
    struct Case {
        const char *name, *rel, *prefix, *data;
    };
    const Case cases[] = {
        {"one-line", ".", "tmp-", "alpha\n"},
        {"many-lines", ".", "report-", "alpha\nbeta\ngamma\n"},
        {"in-subdirectory", "sub", "tmp-", "alpha\n"},
        {"empty-prefix", ".", "", "alpha\n"},
        {"empty-data", ".", "tmp-", ""},
        {"non-ascii-data", ".", "tmp-", "caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac\n"},
        {"prefix-with-spaces", ".", "my temp ", "alpha\n"},
        {"non-ascii-prefix", ".", "caf\xc3\xa9-", "alpha\n"},
    };
    std::error_code ec;
    for (const Case& c : cases) {
        fresh();
        std::string got = files::use_temp_file(dir / c.rel, c.prefix, c.data, ec);
        expect(!ec && got == c.data, c.name);
        expect(count(dir) == 1 && count(dir / "sub") == 0, c.name);
    }

    for (std::string prefix : {"tmp-", "", "my temp ", "caf\xc3\xa9-"}) {
        fresh();
        fs::path path = files::make_temp_file(dir, prefix, ec);
        std::string name = path.filename().string();
        expect(!ec && path.parent_path() == dir && name.starts_with(prefix) && name.size() >= prefix.size() + 6,
               "make: name " + prefix);
        auto st = fs::symlink_status(path);
        expect(fs::is_regular_file(st) && st.permissions() == (fs::perms::owner_read | fs::perms::owner_write) &&
                   fs::file_size(path) == 0,
               "make: mode and size " + prefix);
    }

    fresh();
    fs::path a = files::make_temp_file(dir, "tmp-", ec);
    fs::path b = files::make_temp_file(dir, "tmp-", ec);
    expect(!ec && a != b && count(dir) == 3, "two calls give two files");

    fresh();
    files::use_temp_file(dir / "nodir", "tmp-", "alpha\n", ec);
    expect(ec == std::errc::no_such_file_or_directory, "missing directory");
    files::use_temp_file(dir, "sub/tmp-", "alpha\n", ec);
    expect(ec == std::errc::invalid_argument, "prefix with a slash");
    expect(count(dir / "sub") == 0 && count(dir) == 1, "no file is left");

    fs::remove_all(dir);
    if (failures != 0) return 1;
    std::printf("ok\n");
    return 0;
}
