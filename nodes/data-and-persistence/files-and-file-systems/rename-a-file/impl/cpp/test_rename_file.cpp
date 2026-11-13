// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <system_error>

#include <sys/stat.h>
#include <unistd.h>

#include "rename_file.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path dir = "rename_file_tree";
int failures = 0;

void put(const fs::path& p, const std::string& text) { std::ofstream(p, std::ios::binary) << text; }

std::string get(const fs::path& p) {
    std::ifstream in(p, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), {}};
}

bool lexists(const fs::path& p) {
    std::error_code ec;
    return fs::exists(fs::symlink_status(p, ec));
}

void expect(bool ok, const char* what) {
    if (!ok) {
        std::fprintf(stderr, "%s\n", what);
        ++failures;
    }
}

void fresh() {
    fs::remove_all(dir);
    fs::create_directories(dir / "sub");
}

void run_case(const char* name, const char* src, const char* dst, const char* src_text,
              std::optional<std::string> dst_text, const char* want) {
    fresh();
    if (dst_text) put(dir / dst, *dst_text);
    put(dir / src, src_text);
    std::error_code ec;
    rename_file(dir / src, dir / dst, ec);
    bool ok = !ec && get(dir / dst) == want && (std::string(src) == dst || !lexists(dir / src));
    if (!ok) {
        std::fprintf(stderr, "%s: %s\n", name, ec.message().c_str());
        ++failures;
    }
}

}  // namespace

int main() {
    run_case("rename", "a.txt", "b.txt", "alpha\n", std::nullopt, "alpha\n");
    run_case("replace-existing", "a.txt", "b.txt", "alpha\n", "beta\n", "alpha\n");
    run_case("same-path", "a.txt", "a.txt", "alpha\n", "alpha\n", "alpha\n");
    run_case("empty-file", "a.txt", "b.txt", "", "beta\n", "");
    run_case("other-directory", "a.txt", "sub/a.txt", "alpha\n", std::nullopt, "alpha\n");
    run_case("name-with-spaces", "my notes.txt", "old notes.txt", "alpha\n", std::nullopt, "alpha\n");
    run_case("non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", std::nullopt, "alpha\n");

    fresh();
    std::error_code ec;
    rename_file(dir / "a.txt", dir / "b.txt", ec);
    expect(ec == std::errc::no_such_file_or_directory, "missing src");
    put(dir / "a.txt", "alpha\n");
    rename_file(dir / "a.txt", dir / "no-such-dir" / "b.txt", ec);
    expect(ec == std::errc::no_such_file_or_directory, "missing dst dir");
    fs::create_directory(dir / "dir");
    rename_file(dir / "a.txt", dir / "dir", ec);
    expect(bool(ec) && fs::is_directory(dir / "dir") && get(dir / "a.txt") == "alpha\n", "dst is a directory");

    fs::create_symlink("a.txt", dir / "link");
    rename_file(dir / "link", dir / "new-link", ec);
    expect(!ec && !lexists(dir / "link") && fs::read_symlink(dir / "new-link") == "a.txt", "link");

    put(dir / "b.txt", "beta\n");
    {
        std::ifstream old(dir / "b.txt", std::ios::binary);
        rename_file(dir / "a.txt", dir / "b.txt", ec);
        std::string data{std::istreambuf_iterator<char>(old), {}};
        expect(!ec && data == "beta\n", "open: old data is gone");
    }
    expect(get(dir / "b.txt") == "alpha\n", "open: dst is not the new file");

    struct stat shm{}, tmp{};
    if (stat("/dev/shm", &shm) == 0 && stat(dir.c_str(), &tmp) == 0 && shm.st_dev != tmp.st_dev) {
        fs::path dst = "/dev/shm/rename-file-" + std::to_string(getpid()) + ".txt";
        rename_file(dir / "b.txt", dst, ec);
        expect(ec == std::errc::cross_device_link && lexists(dir / "b.txt") && !lexists(dst), "cross device");
    }

    fs::remove_all(dir);
    if (failures == 0) std::puts("ok");
    return failures == 0 ? 0 : 1;
}
