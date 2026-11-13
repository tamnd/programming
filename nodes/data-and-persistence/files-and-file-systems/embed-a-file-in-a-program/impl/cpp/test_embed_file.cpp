// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <optional>
#include <string_view>

#include "embed_file.hpp"

namespace {

int failures = 0;

void check(const char* name, std::int64_t n, std::optional<std::string_view> want) {
    if (files::embedded_line(n) != want) {
        std::fprintf(stderr, "FAIL %s: line %lld\n", name, static_cast<long long>(n));
        ++failures;
    }
}

// The bytes of the header, to check them against lines.txt.
constexpr unsigned char header[] = {
#include "lines_txt.h"
};

}  // namespace

int main() {
    using namespace std::string_view_literals;
    check("first-line", 1, "alpha"sv);
    check("second-line", 2, "caf\xc3\xa9"sv);
    check("last-line", 4, "gamma"sv);
    check("non-ascii-line", 2, "caf\xc3\xa9"sv);
    check("empty-line", 3, ""sv);
    check("zero", 0, std::nullopt);
    check("negative", -1, std::nullopt);
    check("past-the-end", 5, std::nullopt);

    auto data = files::embedded_data();
    constexpr auto want = "# SPDX-License-Identifier: MIT\nalpha\ncaf\xc3\xa9\n\ngamma\n"sv;
    if (data.size() != 50 || std::memcmp(data.data(), want.data(), want.size()) != 0) {
        std::fprintf(stderr, "FAIL bytes: %zu bytes\n", data.size());
        ++failures;
    }
    if (sizeof header != data.size() || std::memcmp(header, data.data(), data.size()) != 0) {
        std::fprintf(stderr, "FAIL header: lines_txt.h does not match the data\n");
        ++failures;
    }
    if (failures == 0) {
        std::puts("ok");
    }
    return failures == 0 ? 0 : 1;
}
