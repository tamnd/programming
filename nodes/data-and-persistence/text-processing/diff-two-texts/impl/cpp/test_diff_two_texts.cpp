// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

#include "diff_two_texts.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <unistd.h>  // getpid

struct TestCase {
    const char *name, *old_text, *new_text, *want;
};

static const TestCase cases[] = {
    {"simple", "a\nb\nc\n", "a\nc\nd\n", " a\n-b\n c\n+d\n"},
    {"change-a-line", "one\ntwo\nthree\n", "one\n2\nthree\n", " one\n-two\n+2\n three\n"},
    {"identical", "a\nb\n", "a\nb\n", " a\n b\n"},
    {"lines-of-a-poem", "The cat sat\non the mat.\nIt was flat.\n", "The cat sat\non the hat.\nIt was flat.\nThe end.\n", " The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n"},
    {"both-empty", "", "", ""},
    {"old-empty", "", "a\nb\n", "+a\n+b\n"},
    {"new-empty", "a\nb\n", "", "-a\n-b\n"},
    {"all-different", "a\nb\n", "c\nd\n", "-a\n-b\n+c\n+d\n"},
    {"insert-at-start", "b\nc\n", "a\nb\nc\n", "+a\n b\n c\n"},
    {"delete-at-end", "a\nb\nc\n", "a\nb\n", " a\n b\n-c\n"},
    {"swap", "a\nb\n", "b\na\n", "-a\n b\n+a\n"},
    {"repeated-lines", "a\na\nb\n", "a\nb\nb\n", " a\n-a\n b\n+b\n"},
    {"empty-line", "a\n\nb\n", "a\nb\n", " a\n-\n b\n"},
    {"no-final-newline", "a\nb", "a\nb\n", " a\n b\n"},
    {"crlf", "a\r\nb\n", "a\nb\n", "-a\r\n+a\n b\n"},
    {"trailing-space", "a \nb\n", "a\nb\n", "-a \n+a\n b\n"},
    {"normalization", "caf\303\251\n", "cafe\314\201\n", "-caf\303\251\n+cafe\314\201\n"},
};

static void write_file(const std::filesystem::path& path, const std::string& text) {
    std::ofstream(path, std::ios::binary) << text;
}

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (std::string got = diff_lines(c.old_text, c.new_text); got != c.want) {
            std::cerr << "FAIL " << c.name << ": got \"" << got << "\"\n";
            failures++;
        }
    }

    // 1000 lines, and 10 of them change.
    std::string old_text, new_text;
    for (int i = 0; i < 1000; i++) {
        old_text += std::to_string(i) + "\n";
        new_text += (i % 100 == 50 ? "x" : "") + std::to_string(i) + "\n";
    }
    std::string got = diff_lines(old_text, new_text);
    if (std::ranges::count(got, '\n') != 1010 || !got.contains(" 49\n-50\n+x50\n 51\n") || !got.ends_with("\n 999\n")) {
        std::cerr << "FAIL many-lines\n";
        failures++;
    }

    const auto dir = std::filesystem::temp_directory_path() / ("diff-two-texts-cpp-" + std::to_string(::getpid()));
    std::filesystem::create_directories(dir);
    write_file(dir / "old.txt", "a\r\nb\n");
    write_file(dir / "new.txt", "a\nb\n");
    if (auto r = diff_files(dir / "old.txt", dir / "new.txt"); !r || *r != "-a\r\n+a\n b\n") {
        std::cerr << "FAIL diff-files\n";
        failures++;
    }
    if (auto r = diff_files(dir / "missing.txt", dir / "new.txt"); r || r.error() != DiffError::cannot_open) {
        std::cerr << "FAIL not-found\n";
        failures++;
    }
    std::filesystem::remove_all(dir);
    if (failures == 0) {
        std::cout << "ok\n";
    }
    return failures == 0 ? 0 : 1;
}
