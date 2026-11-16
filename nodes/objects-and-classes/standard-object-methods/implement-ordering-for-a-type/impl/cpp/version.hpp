// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

#ifndef VERSION_HPP
#define VERSION_HPP

#include <compare>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace version {

// A version MAJOR.MINOR.PATCH. The defaulted operator<=> compares the members
// in the order of their declaration, and it also gives a defaulted operator==.
struct Version {
    int major = 0;
    int minor = 0;
    int patch = 0;

    auto operator<=>(const Version&) const = default;

    // parse reads a version such as "1.10.0", or returns no value.
    static std::optional<Version> parse(std::string_view text);

    std::string to_string() const;
};

// sort_versions sorts the versions by their natural order. It throws
// std::invalid_argument for a string that is not a version.
std::vector<std::string> sort_versions(const std::vector<std::string>& versions);

}  // namespace version

#endif
