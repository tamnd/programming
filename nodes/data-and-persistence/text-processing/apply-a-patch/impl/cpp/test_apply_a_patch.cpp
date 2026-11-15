// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

#include "apply_a_patch.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <unistd.h>  // getpid

struct TestCase {
    const char *name, *old_text, *patch, *want;
};

static const TestCase cases[] = {
    {"change-a-line", "a\nb\nc\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n a\n-b\n+B\n c\n", "a\nB\nc\n"},
    {"two-hunks", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "--- old.txt\n+++ new.txt\n@@ -1,3 +1,3 @@\n apple\n-banana\n+blueberry\n cherry\n@@ -6,2 +6,2 @@\n fig\n-grape\n+guava\n", "apple\nblueberry\ncherry\ndate\nelder\nfig\nguava\n"},
    {"git-header", "def main():\n    print(1)\n", "diff --git a/main.py b/main.py\nindex 1234567..89abcde 100644\n--- a/main.py\n+++ b/main.py\n@@ -1,2 +1,2 @@ def main():\n def main():\n-    print(1)\n+    print(2)\n", "def main():\n    print(2)\n"},
    {"add-at-end", "a\n", "@@ -1 +1,2 @@\n a\n+b\n", "a\nb\n"},
    {"old-empty", "", "--- /dev/null\n+++ b.txt\n@@ -0,0 +1,2 @@\n+a\n+b\n", "a\nb\n"},
    {"delete-all", "a\nb\n", "@@ -1,2 +0,0 @@\n-a\n-b\n", ""},
    {"no-count", "a\nb\nc\n", "@@ -2 +2 @@\n-b\n+x\n", "a\nx\nc\n"},
    {"zero-context", "apple\nbanana\ncherry\ndate\nelder\nfig\ngrape\n", "@@ -3 +3 @@\n-cherry\n+coconut\n@@ -6 +5,0 @@\n-fig\n@@ -7,0 +7 @@\n+honeydew\n", "apple\nbanana\ncoconut\ndate\nelder\ngrape\nhoneydew\n"},
    {"insert-at-start", "b\n", "@@ -0,0 +1 @@\n+a\n", "a\nb\n"},
    {"old-no-final-newline", "a\nb", "@@ -1,2 +1,2 @@\n a\n-b\n\\ No newline at end of file\n+b\n", "a\nb\n"},
    {"new-no-final-newline", "a\nb\n", "@@ -1,2 +1,2 @@\n a\n-b\n+b\n\\ No newline at end of file\n", "a\nb"},
    {"empty-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n\n-b\n+c\n", "a\n\nc\n"},
    {"space-context-line", "a\n\nb\n", "@@ -1,3 +1,3 @@\n a\n \n-b\n+c\n", "a\n\nc\n"},
    {"crlf", "a\r\nb\r\n", "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n", "a\r\nc\r\n"},
    {"header-like-line", "x\n-- y\n", "--- old.txt\n+++ new.txt\n@@ -1,2 +1 @@\n x\n--- y\n", "x\n"},
    {"non-ascii", "caf\303\251\n", "@@ -1 +1 @@\n-caf\303\251\n+na\303\257ve\n", "na\303\257ve\n"},
};

struct ErrorCase {
    const char *name, *old_text, *patch;
    PatchErrorKind want;
};

static const ErrorCase errors[] = {
    {"mismatch", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-x\n+y\n c\n", PatchErrorKind::mismatch},
    {"malformed", "a\nb\nc\n", "@@ -1,3 +1,3 @@\n a\n-b\n", PatchErrorKind::malformed},
    {"no-hunk", "a\n", "no hunk here\n", PatchErrorKind::malformed},
    {"bad-mark", "a\n", "@@ -1 +1 @@\n*a\n", PatchErrorKind::malformed},
    {"past-the-end", "a\n", "@@ -2 +2 @@\n-a\n+b\n", PatchErrorKind::mismatch},
};

static void write_file(const std::filesystem::path& path, const std::string& text) {
    std::ofstream(path, std::ios::binary) << text;
}

int main() {
    int failures = 0;
    for (const auto& c : cases) {
        if (auto got = apply_patch(c.old_text, c.patch); !got || *got != c.want) {
            std::cerr << "FAIL " << c.name << "\n";
            failures++;
        }
    }
    for (const auto& c : errors) {
        if (auto got = apply_patch(c.old_text, c.patch); got || got.error().kind != c.want) {
            std::cerr << "FAIL " << c.name << "\n";
            failures++;
        }
    }

    // 1000 lines, and 10 hunks with no context change 10 of them.
    std::string old_text, patch, want;
    for (int i = 0; i < 1000; i++) {
        std::string n = std::to_string(i);
        old_text += n + "\n";
        if (i % 100 == 50) {
            std::string at = std::to_string(i + 1);
            patch += "@@ -" + at + " +" + at + " @@\n-" + n + "\n+x" + n + "\n";
            want += "x";
        }
        want += n + "\n";
    }
    if (auto got = apply_patch(old_text, patch); !got || *got != want) {
        std::cerr << "FAIL many-hunks\n";
        failures++;
    }

    const auto dir = std::filesystem::temp_directory_path() / ("apply-a-patch-cpp-" + std::to_string(::getpid()));
    std::filesystem::create_directories(dir);
    write_file(dir / "old.txt", "a\r\nb\r\n");
    write_file(dir / "a.patch", "@@ -1,2 +1,2 @@\n a\r\n-b\r\n+c\r\n");
    if (auto r = apply_patch_file(dir / "old.txt", dir / "a.patch"); !r || *r != "a\r\nc\r\n") {
        std::cerr << "FAIL apply-patch-file\n";
        failures++;
    }
    if (auto r = apply_patch_file(dir / "missing.txt", dir / "a.patch");
        r || r.error().kind != PatchErrorKind::cannot_open) {
        std::cerr << "FAIL not-found\n";
        failures++;
    }
    std::filesystem::remove_all(dir);
    if (failures == 0) {
        std::cout << "ok\n";
    }
    return failures == 0 ? 0 : 1;
}
