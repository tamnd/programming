// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

#include "lock_file.hpp"

namespace fs = std::filesystem;
using files::LockMode;

namespace {

const fs::path dir = "lock_tree";
const fs::path path = dir / "data.txt";
int failures = 0;

void expect(bool ok, const std::string& what) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", what.c_str());
        ++failures;
    }
}

void fresh() {
    fs::remove_all(dir);
    fs::create_directories(dir);
    std::ofstream(path) << "alpha\n";
}

}  // namespace

int main() {
    struct Case {
        const char* name;
        LockMode held, want;
        bool expect;
    };
    const Case cases[] = {
        {"no-lock-exclusive", LockMode::none, LockMode::exclusive, true},
        {"no-lock-shared", LockMode::none, LockMode::shared, true},
        {"exclusive-with-exclusive", LockMode::exclusive, LockMode::exclusive, false},
        {"shared-with-shared", LockMode::shared, LockMode::shared, true},
        {"shared-with-exclusive", LockMode::shared, LockMode::exclusive, false},
        {"exclusive-with-shared", LockMode::exclusive, LockMode::shared, false},
    };
    std::error_code ec;
    for (const Case& c : cases) {
        fresh();
        bool got = files::try_lock(path, c.held, c.want, ec);
        expect(!ec && got == c.expect, c.name);
    }

    fresh();
    {
        auto a = files::try_lock_file(path, LockMode::exclusive, ec);
        expect(a.has_value(), "first lock");
        expect(!files::try_lock_file(path, LockMode::exclusive, ec) && !ec, "second lock is busy");
        pid_t pid = fork();
        if (pid == 0) _exit(files::try_lock_file(path, LockMode::shared, ec) ? 1 : 0);
        int status = 0;
        waitpid(pid, &status, 0);
        expect(WIFEXITED(status) && WEXITSTATUS(status) == 0, "child process sees the lock");
    }
    // The destructor of a released the lock.
    expect(files::try_lock_file(path, LockMode::exclusive, ec).has_value(), "lock after the destructor");

    files::try_lock_file(dir / "missing.txt", LockMode::exclusive, ec);
    expect(ec == std::errc::no_such_file_or_directory && !fs::exists(dir / "missing.txt"), "missing-file");

    fs::remove_all(dir);
    if (failures != 0) return 1;
    std::printf("ok\n");
    return 0;
}
