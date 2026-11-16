// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

#include "read_ports.hpp"

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

std::string quote(const std::string &value) {
    std::string s = "\"";
    for (char c : value) {
        switch (c) {
        case '\\': s += "\\\\"; break;
        case '"': s += "\\\""; break;
        case '\n': s += "\\n"; break;
        case '\r': s += "\\r"; break;
        case '\t': s += "\\t"; break;
        default: s += c;
        }
    }
    return s + "\"";
}

std::vector<int> read_ports(const std::vector<std::string> &texts, std::ostream &log) {
    std::vector<int> ports;
    for (std::size_t i = 0; i < texts.size(); i++) {
        try {
            ports.push_back(parse_port(texts[i]));
        } catch (const std::logic_error &e) {
            // The error is handled here: it is logged once and not thrown again.
            log << "level=ERROR msg=\"read port failed\" line=" << i + 1 << " err=" << quote(e.what()) << "\n";
        }
    }
    log << "level=INFO msg=\"ports read\" ok=" << ports.size() << " total=" << texts.size() << "\n";
    return ports;
}
