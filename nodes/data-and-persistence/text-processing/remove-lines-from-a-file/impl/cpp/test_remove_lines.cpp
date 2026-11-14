// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

#include "remove_lines.hpp"

#include <climits>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

int failures = 0;

void check(bool ok, const std::string& name, const char* what) {
    if (!ok) {
        std::cerr << "FAIL " << name << ": " << what << '\n';
        failures++;
    }
}

void write_file(const fs::path& path, const std::string& text) {
    std::ofstream(path, std::ios::binary) << text;
}

std::string read_file(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

struct Case {
    std::string name, text;
    int start, count, want;
    std::string after;
};

}  // namespace

int main() {
    const fs::path dir = fs::temp_directory_path() / "remove-lines-cpp-test";
    fs::remove_all(dir);
    fs::create_directory(dir);
    const fs::path path = dir / "input.txt";

    std::string many;
    for (int i = 0; i < 100000; i++) {
        many += std::to_string(i) + "\n";
    }
    const std::vector<Case> cases = {
        {"first-two", "a\nb\nc\nd\n", 1, 2, 2, "c\nd\n"},
        {"middle", "a\nb\nc\nd\n", 2, 2, 2, "a\nd\n"},
        {"last", "a\nb\nc\n", 3, 1, 1, "a\nb\n"},
        {"past-the-end", "a\nb\n", 3, 1, 0, "a\nb\n"},
        {"partly-past-the-end", "a\nb\nc\n", 2, 5, 2, "a\n"},
        {"count-zero", "a\nb\n", 1, 0, 0, "a\nb\n"},
        {"all-lines", "a\nb\n", 1, 2, 2, ""},
        {"empty", "", 1, 1, 0, ""},
        {"empty-line", "a\n\nb\n", 2, 1, 1, "a\nb\n"},
        {"no-final-newline", "a\nb", 1, 1, 1, "b"},
        {"remove-incomplete-line", "a\nb", 2, 1, 1, "a\n"},
        {"crlf", "a\r\nb\r\nc\r\n", 2, 1, 1, "a\r\nc\r\n"},
        {"lone-cr", "a\rb\nc\n", 1, 1, 1, "c\n"},
        {"non-ascii", "caf\xc3\xa9\nna\xc3\xafve\nzo\xc3\xab\n", 2, 1, 1, "caf\xc3\xa9\nzo\xc3\xab\n"},
        {"large-count", "a\nb\n", 2, INT_MAX, 1, "a\n"},
        // 100000 lines with the numbers 0 to 99999. Lines 2 to 99999 go.
        {"many-lines", many, 2, 99998, 99998, "0\n99999\n"},
    };
    for (const Case& c : cases) {
        write_file(path, c.text);
        auto got = remove_lines(path, c.start, c.count);
        check(got.has_value() && *got == c.want, c.name, "result");
        check(read_file(path) == c.after, c.name, "file after");
        // No temporary file stays in the directory.
        check(std::distance(fs::directory_iterator(dir), fs::directory_iterator()) == 1, c.name,
              "one file");
    }

    write_file(path, "a\nb\n");
    fs::permissions(path, fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read);
    check(remove_lines(path, 1, 1) == 1, "keeps-the-permissions", "result");
    check(fs::status(path).permissions() ==
              (fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read),
          "keeps-the-permissions", "permissions");

    auto missing = remove_lines(dir / "missing.txt", 1, 1);
    check(!missing && missing.error() == RemoveError::cannot_open, "not-found", "error");
    write_file(path, "a\nb\n");
    auto zero = remove_lines(path, 0, 1);
    check(!zero && zero.error() == RemoveError::bad_range, "start-zero", "error");
    auto negative = remove_lines(path, 1, -1);
    check(!negative && negative.error() == RemoveError::bad_range, "negative-count", "error");
    check(read_file(path) == "a\nb\n", "bad-range", "file after");

    fs::remove_all(dir);
    if (failures == 0) {
        std::cout << "ok\n";
    }
    return failures == 0 ? 0 : 1;
}
