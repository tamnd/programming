// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/watch-a-file-for-changes/.

#include <atomic>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

#include "watch_file.hpp"

namespace fs = std::filesystem;
using namespace std::chrono_literals;

namespace {

const fs::path dir = "watch_tree";
const fs::path path = dir / "watched.txt";
int failures = 0;

void expect(bool ok, const std::string& what) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", what.c_str());
        ++failures;
    }
}

void fresh() {
    fs::remove_all(dir);
    fs::create_directory(dir);
}

struct Case {
    std::string name;
    std::vector<std::string> steps;
    std::vector<std::string> expect;
};

void write_with_mtime(const std::string& text, fs::file_time_type t) {
    std::ofstream(path, std::ios::binary) << text;
    fs::last_write_time(path, t);
}

}  // namespace

int main() {
    const std::vector<Case> cases = {
        {"create", {"create:hello\n"}, {"created"}},
        {"create-then-write", {"create:a\n", "write:a much longer line\n"}, {"created", "modified"}},
        {"create-then-delete", {"create:a\n", "delete"}, {"created", "deleted"}},
        {"append", {"create:a\n", "append:b\n", "append:c\n"}, {"created", "modified", "modified"}},
        {"no-change", {"create:a\n", "none", "none"}, {"created", "none", "none"}},
        {"same-size", {"create:abc\n", "write:xyz\n"}, {"created", "modified"}},
        {"touch-only", {"create:a\n", "touch"}, {"created", "modified"}},
        {"replace-by-rename", {"create:old\n", "replace:new\n"}, {"created", "modified"}},
        {"delete-and-create", {"create:a\n", "recreate:b\n"}, {"created", "modified"}},
        {"never-created", {"none"}, {"none"}},
    };
    for (const auto& c : cases) {
        fresh();
        std::error_code ec;
        auto got = files::watch_changes(path, c.steps, ec);
        expect(!ec && got == c.expect, c.name);
    }

    fresh();
    std::error_code ec;
    files::watch_changes(path, {"mkdir"}, ec);
    expect(ec == std::errc::is_a_directory, "directory");

    // The same size and the same mtime hide a change.
    fresh();
    auto t = fs::file_time_type::clock::now() - 1h;
    write_with_mtime("abc\n", t);
    ec.clear();
    files::FileWatcher watcher(path, ec);
    write_with_mtime("xyz\n", t);
    expect(watcher.poll(ec) == "none", "same size and time");
    std::ofstream(path, std::ios::binary) << "xyz!\n";
    expect(watcher.poll(ec) == "modified", "new size");

    // One call for a burst of changes.
    fresh();
    std::mutex mu;
    std::vector<fs::path> calls;
    std::atomic<bool> done{false};
    std::error_code wec;
    std::thread th([&] {
        files::watch(
            path, [&](const fs::path& p) { std::lock_guard lock(mu); calls.push_back(p); }, 10ms, 500ms,
            [&] { return done.load(); }, wec);
    });
    for (int i = 1; i <= 5; ++i) {
        std::ofstream(path, std::ios::binary) << std::string(i, 'x');
        std::this_thread::sleep_for(20ms);
    }
    auto deadline = std::chrono::steady_clock::now() + 5s;
    while (std::chrono::steady_clock::now() < deadline) {
        {
            std::lock_guard lock(mu);
            if (!calls.empty()) break;
        }
        std::this_thread::sleep_for(10ms);
    }
    std::this_thread::sleep_for(600ms);
    done = true;
    th.join();
    expect(!wec && calls == std::vector<fs::path>{path}, "burst");

    fs::remove_all(dir);
    if (failures) return 1;
    std::puts("all tests pass");
    return 0;
}
