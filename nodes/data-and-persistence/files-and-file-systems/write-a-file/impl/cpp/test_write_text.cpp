// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

#include "write_text.hpp"

#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <stdlib.h>
#include <unistd.h>

namespace fs = std::filesystem;

namespace {

struct Case {
    std::string_view name;
    std::string_view text;
    WriteMode mode;
    std::optional<std::string_view> before;
    std::string_view want;
};

constexpr std::string_view non_ascii = "naïve café\n日本語\n🙂\n";

const std::array cases{
    Case{"new-file", "alpha\nbeta\n", WriteMode::Truncate, std::nullopt, "alpha\nbeta\n"},
    Case{"append-existing", "two\n", WriteMode::Append, "one\n", "one\ntwo\n"},
    Case{"atomic-replace", "new\n", WriteMode::Atomic, "old\n", "new\n"},
    Case{"truncate-longer", "short\n", WriteMode::Truncate, "a much longer old line\n", "short\n"},
    Case{"empty-truncate", "", WriteMode::Truncate, "old\n", ""},
    Case{"empty-new", "", WriteMode::Truncate, std::nullopt, ""},
    Case{"append-missing", "first\n", WriteMode::Append, std::nullopt, "first\n"},
    Case{"append-no-newline", "two", WriteMode::Append, "one", "onetwo"},
    Case{"non-ascii", non_ascii, WriteMode::Truncate, std::nullopt, non_ascii},
    Case{"crlf", "one\r\ntwo\r\n", WriteMode::Truncate, std::nullopt, "one\r\ntwo\r\n"},
    Case{"mixed-endings", "lf\ncrlf\r\ncr\rend", WriteMode::Truncate, std::nullopt, "lf\ncrlf\r\ncr\rend"},
    Case{"backslashes", "C:\\temp\\new\n", WriteMode::Truncate, std::nullopt, "C:\\temp\\new\n"},
    Case{"format-chars", "-n 100% %s\n", WriteMode::Truncate, std::nullopt, "-n 100% %s\n"},
    Case{"atomic-new", "new\n", WriteMode::Atomic, std::nullopt, "new\n"},
    Case{"atomic-empty", "", WriteMode::Atomic, "old\n", ""},
    Case{"atomic-non-ascii", non_ascii, WriteMode::Atomic, "old\n", non_ascii},
};

fs::path make_dir() {
    std::string name = (fs::temp_directory_path() / "write-text-XXXXXX").string();
    std::vector<char> buf(name.begin(), name.end());
    buf.push_back('\0');
    if (mkdtemp(buf.data()) == nullptr) {
        throw std::runtime_error("mkdtemp failed");
    }
    return buf.data();
}

std::vector<std::string> names(const fs::path &dir) {
    std::vector<std::string> out;
    for (const auto &e : fs::directory_iterator(dir)) {
        out.push_back(e.path().filename().string());
    }
    return out;
}

std::string slurp(const fs::path &path) {
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

} // namespace

int main() {
    int failures = 0;
    const std::vector<std::string> only{"out.txt"};
    try {
        for (const Case &c : cases) {
            fs::path dir = make_dir();
            fs::path path = dir / "out.txt";
            if (c.before) {
                write_text(path, *c.before, WriteMode::Truncate);
            }
            try {
                write_text(path, c.text, c.mode);
                if (slurp(path) != c.want) {
                    std::cout << "FAIL " << c.name << ": wrong bytes\n";
                    failures++;
                } else if (names(dir) != only) {
                    std::cout << "FAIL " << c.name << ": other files in the directory\n";
                    failures++;
                }
            } catch (const std::exception &e) {
                std::cout << "FAIL " << c.name << ": " << e.what() << '\n';
                failures++;
            }
            fs::remove_all(dir);
        }

        const std::array<std::pair<std::string_view, WriteMode>, 3> modes{{
            {"truncate", WriteMode::Truncate},
            {"append", WriteMode::Append},
            {"atomic", WriteMode::Atomic},
        }};
        for (const auto &[name, mode] : modes) {
            fs::path dir = make_dir();
            bool threw = false;
            try {
                write_text(dir / "missing" / "out.txt", "x\n", mode);
            } catch (const std::exception &) {
                threw = true;
            }
            if (!threw || !names(dir).empty()) {
                std::cout << "FAIL missing-dir-" << name << '\n';
                failures++;
            }
            fs::remove_all(dir);
        }

        fs::path dir = make_dir();
        fs::path path = dir / "out.txt";
        fs::create_directory(path);
        bool threw = false;
        try {
            write_text(path, "x\n", WriteMode::Atomic);
        } catch (const fs::filesystem_error &) {
            threw = true;
        }
        if (!threw || !fs::is_directory(path) || names(dir) != only) {
            std::cout << "FAIL atomic-onto-directory\n";
            failures++;
        }
        fs::remove_all(dir);
    } catch (const std::exception &e) {
        std::cout << "FAIL setup: " << e.what() << '\n';
        return 1;
    }
    std::cout << failures << " failures\n";
    return failures == 0 ? 0 : 1;
}
