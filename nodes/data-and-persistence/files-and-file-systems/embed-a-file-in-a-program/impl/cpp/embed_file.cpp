// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

#include "embed_file.hpp"

namespace {

// #embed is part of the C++ draft after C++23. A compiler that has it in
// that mode uses it; the others use the same bytes from a header that a tool
// such as xxd made from lines.txt.
constexpr unsigned char data[] = {
#if __cplusplus > 202302L && defined(__has_embed)
#embed "lines.txt"
#else
#include "lines_txt.h"
#endif
};

}  // namespace

namespace files {

std::span<const unsigned char> embedded_data() { return data; }

std::optional<std::string_view> embedded_line(std::int64_t n) {
    std::string_view rest(reinterpret_cast<const char*>(data), sizeof data);
    if (n < 1) {
        return std::nullopt;
    }
    // Skip the first line, the license header of the file.
    auto first = rest.find('\n');
    if (first == std::string_view::npos) {
        return std::nullopt;
    }
    rest.remove_prefix(first + 1);
    for (std::int64_t i = 1; !rest.empty(); ++i) {
        auto nl = rest.find('\n');
        auto line = rest.substr(0, nl);
        if (i == n) {
            return line;
        }
        rest = nl == std::string_view::npos ? std::string_view{} : rest.substr(nl + 1);
    }
    return std::nullopt;
}

}  // namespace files
