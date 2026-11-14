// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

#include "replace_in_files.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
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

struct File {
    std::string name, before, after;
};

struct Case {
    std::string name, old_text, new_text;
    std::vector<File> files;
    std::vector<std::size_t> want;
};

std::string lines(const std::string& prefix) {
    std::string s;
    for (int i = 0; i < 100000; i++) {
        s += prefix + std::to_string(i) + "\n";
    }
    return s;
}

}  // namespace

int main() {
    const std::vector<Case> cases = {
        {"rosetta", "Goodbye London!", "Hello New York!",
         {{"a.txt", "Goodbye London!\n", "Hello New York!\n"},
          {"b.txt", "Goodbye London! Goodbye London!\nGoodbye\n", "Hello New York! Hello New York!\nGoodbye\n"}},
         {1, 2}},
        {"some-files-match", "dog", "fox", {{"a.txt", "cat\n", "cat\n"}, {"b.txt", "dog\ncat dog\n", "fox\ncat fox\n"}}, {0, 2}},
        {"one-file", "1", "2", {{"a.txt", "x = 1\ny = 1\n", "x = 2\ny = 2\n"}}, {2}},
        {"no-files", "a", "b", {}, {}},
        {"no-match", "x", "y", {{"a.txt", "abc\n", "abc\n"}}, {0}},
        {"empty-file", "a", "b", {{"a.txt", "", ""}}, {0}},
        {"delete", "-", "", {{"a.txt", "a-b-c\n", "abc\n"}}, {2}},
        {"whole-file", "abc", "", {{"a.txt", "abc", ""}}, {1}},
        {"new-contains-old", "a", "aa", {{"a.txt", "a a\n", "aa aa\n"}}, {2}},
        {"overlapping", "aa", "b", {{"a.txt", "aaa\n", "ba\n"}}, {1}},
        {"adjacent", "aa", "b", {{"a.txt", "aaaa\n", "bb\n"}}, {2}},
        {"special-chars", "1.5", "\\&$1 $& [x]", {{"a.txt", "1.5 and 125\n", "\\&$1 $& [x] and 125\n"}}, {1}},
        {"glob-chars", "*.txt", "?", {{"a.txt", "a.txt *.txt\n", "a.txt ?\n"}}, {1}},
        {"across-lines", "end\nstart", "end start", {{"a.txt", "end\nstart\n", "end start\n"}}, {1}},
        {"lf-to-crlf", "\n", "\r\n", {{"a.txt", "a\nb\n", "a\r\nb\r\n"}}, {2}},
        {"crlf", "a", "c", {{"a.txt", "a\r\nb\r\n", "c\r\nb\r\n"}}, {1}},
        {"no-final-newline", "b", "c", {{"a.txt", "a b", "a c"}}, {1}},
        {"non-ascii", "\xc3\xa9", "e", {{"a.txt", "caf\xc3\xa9" ", cafe\n", "cafe, cafe\n"}}, {1}},
        {"case-sensitive", "hello", "bye", {{"a.txt", "Hello hello\n", "Hello bye\n"}}, {1}},
    };
    const fs::path root = fs::temp_directory_path() / ("replace-in-files-" + std::to_string(std::random_device{}()));
    fs::create_directories(root);
    int n = 0;
    for (const Case& c : cases) {
        const fs::path dir = root / std::to_string(n++);
        fs::create_directory(dir);
        std::vector<fs::path> paths;
        for (const File& f : c.files) {
            paths.push_back(dir / f.name);
            write_file(paths.back(), f.before);
        }
        auto got = replace_in_files(paths, c.old_text, c.new_text);
        check(got.has_value() && *got == c.want, c.name, "result");
        for (const File& f : c.files) {
            check(read_file(dir / f.name) == f.after, c.name, "after");
        }
        // No temporary file stays in the directory.
        auto entries = std::distance(fs::directory_iterator(dir), fs::directory_iterator());
        check(entries == static_cast<std::ptrdiff_t>(c.files.size()), c.name, "files");
    }

    const fs::path many = root / "many.txt";
    write_file(many, lines("line "));
    auto got = replace_in_files({many}, "line ", "L");
    check(got.has_value() && *got == std::vector<std::size_t>{100000}, "many-lines", "result");
    check(read_file(many) == lines("L"), "many-lines", "after");

    const fs::path a = root / "a.txt";
    write_file(a, "a\n");
    fs::permissions(a, fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read);
    got = replace_in_files({a}, "a", "b");
    check(got.has_value() && (*got)[0] == 1, "keeps-the-permissions", "result");
    check(fs::status(a).permissions() == (fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read),
          "keeps-the-permissions", "mode");

    got = replace_in_files({root / "missing.txt"}, "a", "b");
    check(!got.has_value() && got.error() == ReplaceError::cannot_open, "not-found", "error");
    got = replace_in_files({a}, "", "b");
    check(!got.has_value() && got.error() == ReplaceError::empty_old, "empty-old", "error");
    check(read_file(a) == "b\n", "empty-old", "after");

    const fs::path stop = root / "stop";
    fs::create_directory(stop);
    write_file(stop / "a.txt", "a\n");
    write_file(stop / "c.txt", "a\n");
    got = replace_in_files({stop / "a.txt", stop / "missing.txt", stop / "c.txt"}, "a", "b");
    check(!got.has_value() && got.error() == ReplaceError::cannot_open, "stop", "error");
    check(read_file(stop / "a.txt") == "b\n" && read_file(stop / "c.txt") == "a\n", "stop", "after");

    fs::remove_all(root);
    if (failures == 0) {
        std::cout << "ok\n";
    }
    return failures != 0;
}
