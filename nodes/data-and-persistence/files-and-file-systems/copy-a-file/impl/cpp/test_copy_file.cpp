// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <system_error>

#include "copy_file.hpp"

namespace fs = std::filesystem;

namespace {

const fs::path dir = "copy_file_tree";
int failures = 0;

void put(const fs::path& p, const std::string& text) { std::ofstream(p, std::ios::binary) << text; }

std::string get(const fs::path& p) {
    std::ifstream in(p, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), {}};
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

void run_case(const char* name, const char* src, const char* dst, const std::string& src_text,
              std::optional<std::string> dst_text) {
    fresh();
    if (dst_text) put(dir / dst, *dst_text);
    put(dir / src, src_text);
    std::error_code ec;
    files::copy_file(dir / src, dir / dst, ec);
    if (ec || get(dir / dst) != src_text || get(dir / src) != src_text) {
        std::fprintf(stderr, "%s: %s\n", name, ec.message().c_str());
        ++failures;
    }
}

bool fails(const fs::path& src, const fs::path& dst, std::errc want) {
    std::error_code ec;
    files::copy_file(src, dst, ec);
    return ec == want;
}

void test_errors() {
    fresh();
    expect(fails(dir / "a.txt", dir / "b.txt", std::errc::no_such_file_or_directory), "missing src");
    put(dir / "a.txt", "alpha\n");
    expect(fails(dir / "a.txt", dir / "no-such-dir/b.txt", std::errc::no_such_file_or_directory),
           "missing dst dir");
    expect(fails(dir / "sub", dir / "b.txt", std::errc::is_a_directory), "src dir");
    expect(fails(dir / "a.txt", dir / "sub", std::errc::is_a_directory), "dst dir");
    expect(fs::is_empty(dir / "sub") && !fs::exists(dir / "b.txt"), "no change");
    fs::create_hard_link(dir / "a.txt", dir / "c.txt");
    expect(fails(dir / "a.txt", dir / "a.txt", std::errc::invalid_argument), "same path");
    expect(fails(dir / "a.txt", dir / "c.txt", std::errc::invalid_argument), "hard link");
    expect(get(dir / "a.txt") == "alpha\n", "same file keeps its data");
}

void test_mode() {
    fresh();
    put(dir / "a.sh", "echo alpha\n");
    put(dir / "c.sh", "beta\n");
    auto bits = fs::perms::owner_all | fs::perms::group_read | fs::perms::group_exec;
    fs::permissions(dir / "a.sh", bits);
    fs::permissions(dir / "c.sh", fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read);
    for (const char* name : {"b.sh", "c.sh"}) {
        std::error_code ec;
        files::copy_file(dir / "a.sh", dir / name, ec);
        expect(!ec && fs::status(dir / name).permissions() == bits, name);
    }
}

void test_large() {
    fresh();
    std::string data(5000003, '\0');
    for (std::size_t i = 0; i < data.size(); ++i) data[i] = static_cast<char>(i * 7 % 251);
    put(dir / "big.bin", data);
    std::error_code ec;
    files::copy_file(dir / "big.bin", dir / "copy.bin", ec);
    expect(!ec && get(dir / "copy.bin") == data, "large");
}

void test_link_at_src() {
    fresh();
    put(dir / "target.txt", "alpha\n");
    fs::create_symlink("target.txt", dir / "link.txt");
    std::error_code ec;
    files::copy_file(dir / "link.txt", dir / "b.txt", ec);
    expect(!ec && fs::is_regular_file(fs::symlink_status(dir / "b.txt")), "link gives a file");
    expect(get(dir / "b.txt") == "alpha\n", "link data");
}

}  // namespace

int main() {
    using namespace std::string_literals;
    run_case("copy", "a.txt", "b.txt", "alpha\n", std::nullopt);
    run_case("replace-existing", "a.txt", "b.txt", "alpha\n", "beta, a longer text\n");
    run_case("empty-file", "a.txt", "b.txt", "", "beta\n");
    run_case("other-directory", "a.txt", "sub/a.txt", "alpha\n", std::nullopt);
    run_case("name-with-spaces", "my notes.txt", "copy of my notes.txt", "alpha\n", std::nullopt);
    run_case("non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", std::nullopt);
    run_case("binary-data", "a.bin", "b.bin", "\0\r\nÿ\0"s, std::nullopt);
    test_errors();
    test_mode();
    test_large();
    test_link_at_src();
    fs::remove_all(dir);
    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    std::puts("ok");
    return 0;
}
