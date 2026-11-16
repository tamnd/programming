// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

#include "words.hpp"

namespace words {

int word_count(std::string_view text) {
    int count = 0;
    bool in_word = false;
    for (char c : text) {
        const bool space = c == ' ' || c == '\t' || c == '\n' || c == '\r';
        if (!space && !in_word) {
            count++;
        }
        in_word = !space;
    }
    return count;
}

}  // namespace words
