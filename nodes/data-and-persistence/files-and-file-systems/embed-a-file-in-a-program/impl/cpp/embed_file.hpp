// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

namespace files {

// The bytes of lines.txt, put in the program when it is compiled.
std::span<const unsigned char> embedded_data();

// Returns line n of lines.txt (1 is the line after the license header),
// with no line end, or
// std::nullopt when there is no such line. The view points into the
// embedded data, so it stays valid for the whole run of the program.
std::optional<std::string_view> embedded_line(std::int64_t n);

}  // namespace files
