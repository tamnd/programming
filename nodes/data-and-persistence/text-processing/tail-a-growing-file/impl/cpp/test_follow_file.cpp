// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/tail-a-growing-file/.

#include "follow_file.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <unistd.h>
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

// Each poll is one string, with an LF after each line.
struct Case {
    std::string name, initial;
    std::vector<std::string> steps;
    std::vector<std::string> want;
};

const std::vector<Case> cases = {
    {"append-lines", "old\n", {"append:a\nb\n", "poll", "append:c\n", "poll"}, {"a\nb\n", "c\n"}},
    {"partial-line", "", {"append:ab", "poll", "append:c\nd", "poll", "append:\n", "poll"}, {"", "abc\n", "d\n"}},
    {"rotate", "1\n", {"append:2\n", "poll", "rotate", "append:3\n", "poll"}, {"2\n", "3\n"}},
    {"truncate", "", {"append:a\nb\n", "poll", "truncate:", "poll", "append:c\n", "poll"}, {"a\nb\n", "", "c\n"}},
    {"no-change", "x\n", {"poll", "poll"}, {"", ""}},
    {"empty-line", "", {"append:\n\na\n", "poll"}, {"\n\na\n"}},
    {"crlf", "", {"append:a\r\nb\r\n", "poll"}, {"a\r\nb\r\n"}},
    {"truncate-and-write", "", {"append:aaaa\n", "poll", "truncate:b\n", "poll"}, {"aaaa\n", "b\n"}},
    {"truncate-partial", "", {"append:abc", "poll", "truncate:d\n", "poll"}, {"", "abc\nd\n"}},
    {"rotate-late-writes", "", {"append:1\n", "poll", "rotate", "append-old:2\n", "append:3\n", "poll"}, {"1\n", "2\n3\n"}},
    {"rotate-partial", "", {"append:a", "poll", "rotate", "append:b\n", "poll"}, {"", "a\nb\n"}},
    {"rotate-twice", "", {"append:1\n", "poll", "rotate", "append:2\n", "poll", "rotate", "append:3\n", "poll"}, {"1\n", "2\n", "3\n"}},
    {"delete", "", {"append:1\n", "poll", "delete", "poll", "poll"}, {"1\n", "", ""}},
    {"delete-partial", "", {"append:x", "poll", "delete", "poll"}, {"", "x\n"}},
    {"delete-and-create", "", {"append:1\n", "poll", "delete", "poll", "create:2\n", "poll"}, {"1\n", "", "2\n"}},
    {"delete-and-create-between-polls", "", {"append:1\n", "poll", "delete", "create:2\n", "poll"}, {"1\n", "2\n"}},
    {"start-mid-line", "abc", {"append:d\ne\n", "poll"}, {"d\ne\n"}},
    {"non-ascii", "", {"append:caf\303\251\nna\303\257ve\n", "poll"}, {"caf\303\251\nna\303\257ve\n"}},
};

std::vector<std::string> join(const std::vector<std::vector<std::string>>& polls) {
    std::vector<std::string> joined;
    for (const auto& lines : polls) {
        std::string s;
        for (const auto& line : lines) {
            s += line + '\n';
        }
        joined.push_back(s);
    }
    return joined;
}

}  // namespace

int main() {
    fs::path dir = fs::temp_directory_path() / ("follow-file-" + std::to_string(getpid()));
    fs::create_directory(dir);
    fs::path path = dir / "followed.txt";

    for (const auto& c : cases) {
        check(join(follow_file(path, c.initial, c.steps)) == c.want, c.name, "polls");
        fs::remove_all(dir);
        fs::create_directory(dir);
    }

    // not-found
    try {
        follow_file(dir / "missing" / "followed.txt", "", {"poll"});
        check(false, "not-found", "no error");
    } catch (const std::system_error& e) {
        check(e.code() == std::errc::no_such_file_or_directory, "not-found", "code");
    }
    try {
        Follower follower(dir / "missing.txt");
        check(false, "not-found", "no error");
    } catch (const std::system_error& e) {
        check(e.code() == std::errc::no_such_file_or_directory, "not-found", "code");
    }

    // 100000 lines in one poll.
    std::string text = "append:";
    for (int i = 0; i < 100000; i++) {
        text += std::to_string(i) + '\n';
    }
    auto polls = follow_file(path, "", {text, "poll"});
    check(polls.size() == 1 && polls[0].size() == 100000, "many-lines", "size");
    check(polls[0].front() == "0" && polls[0].back() == "99999", "many-lines", "lines");
    fs::remove(path);

    // A character across two polls.
    polls = follow_file(path, "", {"append:caf\xc3", "poll", "append:\xa9\n", "poll"});
    check(join(polls) == std::vector<std::string>{"", "caf\xc3\xa9\n"}, "character-across-polls", "polls");
    fs::remove(path);

    // follow: the lines that come after the follower starts.
    std::ofstream(path, std::ios::binary) << "old\n";
    Follower follower(path);
    std::ofstream(path, std::ios::binary | std::ios::app) << "a\nb\n";
    std::vector<std::string> lines;
    int n = 0;
    follower.follow(
        std::chrono::milliseconds(10), [&] { return lines.size() >= 2 || ++n > 500; },
        [&](std::string line) { lines.push_back(std::move(line)); });
    check(lines == std::vector<std::string>{"a", "b"}, "follow", "lines");

    fs::remove_all(dir);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all tests pass\n";
    return 0;
}
