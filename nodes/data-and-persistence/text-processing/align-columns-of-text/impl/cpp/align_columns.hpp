// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

#pragma once

#include <string>
#include <string_view>

// Aligns the fields of the text in columns. A $ separates two fields, and
// align is "left", "right" or "center". Throws std::invalid_argument if
// align is not one of these words.
std::string align_columns(std::string_view text, std::string_view align);
