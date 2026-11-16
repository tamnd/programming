// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

#include "check_setting.hpp"

#include <algorithm>
#include <stdexcept>

int parse_port(const std::string &text) {
    if (text.empty() || !std::all_of(text.begin(), text.end(), [](char c) { return c >= '0' && c <= '9'; })) {
        throw std::invalid_argument("not a number: '" + text + "'");
    }
    long value = 0;
    for (char c : text) {
        value = value * 10 + (c - '0');
        if (value > 65535) {
            throw std::out_of_range("out of range: " + text);
        }
    }
    if (value == 0) {
        throw std::out_of_range("out of range: " + text);
    }
    return static_cast<int>(value);
}

int read_port(const std::string &where, const std::string &text) {
    try {
        return parse_port(text);
    } catch (const std::logic_error &) {
        // throw_with_nested keeps the exception that is handled now as the
        // nested exception of the new one.
        std::throw_with_nested(std::runtime_error(where + ": read port"));
    }
}

std::string chain(const std::exception &e) {
    std::string message = e.what();
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception &inner) {
        message += ": " + chain(inner);
    }
    return message;
}

std::string check_setting(const std::string &where, const std::string &text) {
    try {
        int port = read_port(where, text);
        return "port " + std::to_string(port);
    } catch (const std::runtime_error &e) {
        return "error: " + chain(e);
    }
}
