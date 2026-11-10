// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

#include <cstdio>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>

#include "list_entries.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path root = "list_entries_tree";

void make_tree(const std::vector<std::string>& tree) {
    fs::remove_all(root);
    fs::create_directory(root);
    for (const auto& entry : tree) {
        if (auto arrow = entry.find(" -> "); arrow != std::string::npos) {
            fs::create_symlink(entry.substr(arrow + 4), root / entry.substr(0, arrow));
        } else if (entry.ends_with('/')) {
            fs::create_directory(root / entry);
        } else {
            std::ofstream{root / entry};
        }
    }
}

int failures = 0;

void check(const std::string& name, bool ok) {
    if (!ok) {
        std::fprintf(stderr, "case %s failed\n", name.c_str());
        ++failures;
    }
}

std::errc code_of(const fs::path& path) {
    try {
        list_entries(path, true);
    } catch (const fs::filesystem_error& e) {
        return static_cast<std::errc>(e.code().value());
    }
    return {};
}

}  // namespace

int main() {
    struct Case {
        std::string name;
        std::vector<std::string> tree;
        bool recursive;
        std::vector<std::string> want;
    };
    const std::vector<std::string> tree = {"docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"};
    const std::vector<Case> cases = {
        {"flat", {"b.txt", "a.txt", "c.txt"}, false, {"a.txt", "b.txt", "c.txt"}},
        {"nested", tree, true, tree},
        {"nested-shallow", tree, false, {"docs/", "readme.md"}},
        {"empty", {}, true, {}},
        {"empty-subdir", {"empty/"}, true, {"empty/"}},
        {"hidden", {".env", ".git/", ".git/config", "main.go"}, true, {".env", ".git/", ".git/config", "main.go"}},
        {"case-order", {"a.txt", "B.txt", "c.txt"}, false, {"B.txt", "a.txt", "c.txt"}},
        {"dot-before-slash", {"a/", "a/x", "a.txt", "a-b"}, true, {"a-b", "a.txt", "a/", "a/x"}},
        {"symlink-to-dir", {"real/", "real/f", "link -> real"}, true, {"link", "real/", "real/f"}},
        {"symlink-to-file", {"f", "g -> f"}, true, {"f", "g"}},
        {"broken-symlink", {"dangling -> nowhere"}, true, {"dangling"}},
        {"symlink-loop", {"d/", "d/up -> .."}, true, {"d/", "d/up"}},
        {"spaces", {"my file.txt", "two  spaces/", "two  spaces/x"}, true, {"my file.txt", "two  spaces/", "two  spaces/x"}},
        {"non-ascii", {"été.txt", "zebra.txt", "apple.txt"}, false, {"apple.txt", "zebra.txt", "été.txt"}},
    };
    for (const auto& c : cases) {
        make_tree(c.tree);
        check(c.name, list_entries(root, c.recursive) == c.want);
    }

    make_tree({"notes.txt"});
    check("missing", code_of(root / "missing") == std::errc::no_such_file_or_directory);
    check("not-a-directory", code_of(root / "notes.txt") == std::errc::not_a_directory);
    fs::remove_all(root);
    return failures == 0 ? 0 : 1;
}
