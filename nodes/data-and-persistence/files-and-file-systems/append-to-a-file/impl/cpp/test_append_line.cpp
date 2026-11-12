// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "append_line.hpp"

namespace {

const std::filesystem::path log_path = "append_line_log.txt";

std::string read_file() {
    std::ifstream in(log_path, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

int failures = 0;

void check(const std::string& name, bool ok) {
    if (!ok) {
        std::fprintf(stderr, "case %s failed\n", name.c_str());
        ++failures;
    }
}

std::string text(int w, int i) {
    return std::format("writer {} line {:03} ", w, i) + std::string(40, 'x');
}

}  // namespace

int main() {
    struct Case {
        std::string name;
        std::optional<std::string> old;  // nothing when no file is there
        std::string line, want;
    };
    const std::vector<Case> cases = {
        {"new-file", std::nullopt, "first", "first\n"},
        {"append-existing", "one\n", "two", "one\ntwo\n"},
        {"new-file-empty-line", std::nullopt, "", "\n"},
        {"empty-line", "a\n", "", "a\n\n"},
        {"empty-file", "", "x", "x\n"},
        {"no-final-newline", "one", "two", "onetwo\n"},
        {"crlf-file", "one\r\n", "two", "one\r\ntwo\n"},
        {"whitespace", "", "  two spaces\ttab ", "  two spaces\ttab \n"},
        {"non-ascii", "naïve\n", "café 日本語 🙂", "naïve\ncafé 日本語 🙂\n"},
    };
    for (const auto& c : cases) {
        std::filesystem::remove(log_path);
        if (c.old) std::ofstream(log_path, std::ios::binary) << *c.old;
        check(c.name, append_line(log_path, c.line).has_value() && read_file() == c.want);
    }

    std::filesystem::remove(log_path);
    check("missing-dir", append_line("missing_dir/log.txt", "x").error_or(AppendError::write_failed) ==
                             AppendError::cannot_open);
    for (std::string line : {"a\nb", "a\rb"}) {
        check("invalid-line", append_line(log_path, line).error_or(AppendError::write_failed) ==
                                  AppendError::invalid_line &&
                                  !std::filesystem::exists(log_path));
    }

    const int writers = 8, lines = 250;
    std::vector<std::thread> threads;
    std::vector<int> errors(writers, 0);
    for (int w = 0; w < writers; ++w) {
        threads.emplace_back([w, &errors] {
            for (int i = 0; i < lines; ++i) {
                if (!append_line(log_path, text(w, i))) ++errors[w];
            }
        });
    }
    for (auto& t : threads) t.join();
    std::vector<std::string> got, want;
    std::istringstream in(read_file());
    for (std::string s; std::getline(in, s);) got.push_back(s);
    for (int w = 0; w < writers; ++w) {
        for (int i = 0; i < lines; ++i) want.push_back(text(w, i));
    }
    std::ranges::sort(got);
    std::ranges::sort(want);
    check("many-writers", got == want && std::ranges::count(errors, 0) == writers);
    std::filesystem::remove(log_path);

    if (failures != 0) return EXIT_FAILURE;
    std::puts("all tests passed");
    return EXIT_SUCCESS;
}
