// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

#include "version.hpp"

#include <algorithm>
#include <charconv>
#include <stdexcept>

namespace version {

std::optional<Version> Version::parse(std::string_view text) {
    int parts[3];
    const char* p = text.data();
    const char* end = text.data() + text.size();
    for (int i = 0; i < 3; ++i) {
        if (i > 0) {
            if (p == end || *p != '.') return std::nullopt;
            ++p;
        }
        if (p == end || *p < '0' || *p > '9') return std::nullopt;
        auto [next, ec] = std::from_chars(p, end, parts[i]);
        if (ec != std::errc{}) return std::nullopt;
        p = next;
    }
    if (p != end) return std::nullopt;
    return Version{parts[0], parts[1], parts[2]};
}

std::string Version::to_string() const {
    return std::to_string(major) + '.' + std::to_string(minor) + '.' + std::to_string(patch);
}

std::vector<std::string> sort_versions(const std::vector<std::string>& versions) {
    std::vector<Version> vs;
    vs.reserve(versions.size());
    for (const auto& s : versions) {
        auto v = Version::parse(s);
        if (!v) throw std::invalid_argument("not a version: " + s);
        vs.push_back(*v);
    }
    std::sort(vs.begin(), vs.end());  // uses operator<, which the compiler rewrites to <=>
    std::vector<std::string> out;
    out.reserve(vs.size());
    for (const auto& v : vs) out.push_back(v.to_string());
    return out;
}

}  // namespace version
