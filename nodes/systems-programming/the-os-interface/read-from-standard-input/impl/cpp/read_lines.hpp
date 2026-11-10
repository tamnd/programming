// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

#pragma once

#include <iostream>
#include <string>
#include <vector>

// Reads the stream to its end and returns its lines, each without "\n" or "\r\n".
// The default stream is standard input. A read error throws std::ios_base::failure.
std::vector<std::string> read_lines(std::istream& in = std::cin);
