// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

#pragma once

#include <stdexcept>
#include <string>

// PortError reports that a text is not a valid TCP port number.
class PortError : public std::runtime_error {
public:
    PortError(std::string text, std::string reason);

    const std::string &text() const noexcept { return text_; }
    const std::string &reason() const noexcept { return reason_; }

private:
    std::string text_;
    std::string reason_;
};

// parse_port returns the port number in text, or throws PortError.
int parse_port(const std::string &text);

// check_port returns "port N", or the message of the PortError.
std::string check_port(const std::string &text);
