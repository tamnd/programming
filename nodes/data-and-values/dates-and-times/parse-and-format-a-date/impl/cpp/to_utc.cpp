// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

#include "to_utc.hpp"

#include <charconv>
#include <chrono>
#include <format>
#include <regex>
#include <string>
#include <string_view>

namespace {

// The date-time of RFC 3339 section 5.6, with a fraction of 1 to 3 digits.
// The groups: year, month, day, hour, minute, second, fraction, Z or offset,
// offset sign, offset hours, offset minutes.
const std::regex rfc3339(
    "([0-9]{4})-([0-9]{2})-([0-9]{2})[Tt]([01][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])"
    "(?:\\.([0-9]{1,3}))?([Zz]|([+-])([01][0-9]|2[0-3]):([0-5][0-9]))");

int number(const std::ssub_match& group) {
    std::string_view digits(group.first, group.second);
    int n = 0;
    std::from_chars(digits.data(), digits.data() + digits.size(), n);
    return n;
}

}  // namespace

std::expected<std::string, std::string> to_utc(std::string_view timestamp) {
    using namespace std::chrono;
    std::string text(timestamp);
    std::smatch m;
    if (!std::regex_match(text, m, rfc3339)) {
        return std::unexpected("not an RFC 3339 date-time: \"" + text + "\"");
    }
    // ok() is false for a date that is not in the calendar, as 2026-02-29.
    year_month_day date{year{number(m[1])}, month{static_cast<unsigned>(number(m[2]))},
                        day{static_cast<unsigned>(number(m[3]))}};
    if (!date.ok()) {
        return std::unexpected("not a date in the calendar: \"" + text + "\"");
    }
    // The fraction: 1 to 3 digits, padded to milliseconds.
    std::string fraction = m[7].str();
    fraction.resize(3, '0');
    sys_time<milliseconds> local = sys_days{date} + hours{number(m[4])} +
                                   minutes{number(m[5])} + seconds{number(m[6])} +
                                   milliseconds{std::stoi(fraction)};
    // The offset is local time minus UTC. -00:00 is the same instant as Z.
    minutes offset{0};
    if (m[9].matched) {
        offset = hours{number(m[10])} + minutes{number(m[11])};
        if (m[9] == "-") {
            offset = -offset;
        }
    }
    // %F is %Y-%m-%d, and %T prints the seconds with the 3 digits of
    // milliseconds.
    return std::format("{:%FT%T}Z", local - offset);
}
