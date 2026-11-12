// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

#include "parse_args.hpp"

#include <charconv>
#include <cstddef>
#include <optional>
#include <system_error>

namespace {

// from_chars takes no '+' and no white space; the first check rejects '-'.
std::optional<int> decimal(std::string_view s) {
    if (s.empty() || s.front() < '0' || s.front() > '9') return std::nullopt;
    int n = 0;
    auto [end, ec] = std::from_chars(s.data(), s.data() + s.size(), n);
    if (ec != std::errc{} || end != s.data() + s.size()) return std::nullopt;
    return n;
}

}  // namespace

std::expected<CountArgs, std::string> parse_args(std::span<const std::string_view> args) {
    CountArgs out;
    std::size_t i = 0;
    for (; i < args.size(); ++i) {
        std::string_view arg = args[i];
        std::optional<std::string_view> value;
        if (arg == "--") {
            ++i;
            break;
        } else if (arg == "-v" || arg == "--verbose") {
            out.verbose = true;
            continue;
        } else if (arg == "-n" || arg == "--limit") {
            if (i + 1 == args.size()) {
                return std::unexpected("option " + std::string(arg) + " needs a value");
            }
            value = args[++i];
        } else if (arg.starts_with("--limit=")) {
            value = arg.substr(std::string_view("--limit=").size());
        } else if (arg.starts_with('-') && arg != "-") {
            return std::unexpected("unknown option " + std::string(arg));
        } else {
            break;  // The first operand ends the options.
        }
        auto n = decimal(*value);
        if (!n) return std::unexpected("limit '" + std::string(*value) + "' is not a decimal number");
        out.limit = *n;
    }
    out.files.assign(args.begin() + static_cast<std::ptrdiff_t>(i), args.end());
    return out;
}
