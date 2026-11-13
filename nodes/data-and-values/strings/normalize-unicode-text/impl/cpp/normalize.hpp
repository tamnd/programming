// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/normalize-unicode-text/.
#ifndef NORMALIZE_HPP
#define NORMALIZE_HPP

#include <string>
#include <string_view>

// normalize returns the UTF-8 string s in the form "NFC" or "NFD", for the
// code points of the contract.
std::string normalize(std::string_view s, std::string_view form);

#endif
