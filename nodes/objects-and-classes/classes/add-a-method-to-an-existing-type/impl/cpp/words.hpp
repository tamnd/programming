// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

#ifndef WORDS_HPP
#define WORDS_HPP

#include <string_view>

// C++ cannot add a member function to std::string. A free function in the
// namespace of the code takes the string as a parameter instead.
namespace words {

// word_count returns the number of runs of characters in text that are not
// space, tab, newline or carriage return.
int word_count(std::string_view text);

}  // namespace words

#endif
