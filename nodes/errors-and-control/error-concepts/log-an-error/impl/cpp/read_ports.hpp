// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

#pragma once

#include <ostream>
#include <string>
#include <vector>

// parse_port returns the port number in text. It throws std::invalid_argument
// or std::out_of_range.
int parse_port(const std::string &text);

// quote puts value in double quotes and escapes the characters that could end
// the line or the value.
std::string quote(const std::string &value);

// read_ports returns the good ports. It writes a log line to log for each bad
// text, then a summary.
std::vector<int> read_ports(const std::vector<std::string> &texts, std::ostream &log);
