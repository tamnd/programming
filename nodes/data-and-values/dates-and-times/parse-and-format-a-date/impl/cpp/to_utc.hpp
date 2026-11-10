// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

#ifndef TO_UTC_HPP
#define TO_UTC_HPP

#include <expected>
#include <string>
#include <string_view>

// to_utc returns the instant of an RFC 3339 timestamp in UTC, as
// YYYY-MM-DDTHH:MM:SS.sssZ, or an error message when timestamp is not an RFC
// 3339 date-time with an offset, or when its date is not in the calendar.
std::expected<std::string, std::string> to_utc(std::string_view timestamp);

#endif
