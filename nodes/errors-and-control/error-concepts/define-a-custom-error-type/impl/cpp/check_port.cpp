// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

#include "check_port.hpp"

#include <algorithm>
#include <utility>

PortError::PortError(std::string text, std::string reason)
    : std::runtime_error("invalid port '" + text + "': " + reason),
      text_(std::move(text)),
      reason_(std::move(reason)) {}

int parse_port(const std::string &text) {
    if (text.empty()) {
        throw PortError(text, "empty");
    }
    if (!std::all_of(text.begin(), text.end(), [](char c) { return c >= '0' && c <= '9'; })) {
        throw PortError(text, "not a number");
    }
    long value = 0;
    for (char c : text) {
        value = value * 10 + (c - '0');
        if (value > 65535) {
            throw PortError(text, "out of range");
        }
    }
    if (value == 0) {
        throw PortError(text, "out of range");
    }
    return static_cast<int>(value);
}

std::string check_port(const std::string &text) {
    try {
        int port = parse_port(text);
        return "port " + std::to_string(port);
    } catch (const PortError &e) {
        // Catch by reference to const, so the object is not sliced or copied.
        return e.what();
    }
}
