// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

#ifndef RANDOM_NUMBERS_HPP
#define RANDOM_NUMBERS_HPP

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>

namespace random_numbers {

// A word source returns a random integer from 0 to 2^32 - 1. std::mt19937 is one.
template <typename W>
concept word_source = std::invocable<W&> && std::convertible_to<std::invoke_result_t<W&>, std::uint64_t>;

inline constexpr std::uint64_t words = std::uint64_t{1} << 32;

// Returns an integer from lo to hi, each with equal probability. A word below
// 2^32 % n is rejected, so that each remainder comes from the same number of
// words. Throws std::invalid_argument if hi < lo or the range has more than
// 2^32 values.
template <word_source W>
std::int64_t int_in_range(W& next_word, std::int64_t lo, std::int64_t hi) {
    // The unsigned difference is exact, also when hi - lo overflows.
    const std::uint64_t span = static_cast<std::uint64_t>(hi) - static_cast<std::uint64_t>(lo);
    if (hi < lo || span >= words) {
        throw std::invalid_argument("need lo <= hi and at most 2^32 values");
    }
    const std::uint64_t n = span + 1;
    const std::uint64_t least = words % n;
    for (;;) {
        const std::uint64_t word = static_cast<std::uint64_t>(next_word()) & (words - 1);
        if (word >= least) {
            // The sum is at most hi, so the conversion back to int64 is exact.
            return static_cast<std::int64_t>(static_cast<std::uint64_t>(lo) + word % n);
        }
    }
}

// Shuffles items in place with the Fisher-Yates method.
template <typename T, word_source W>
void shuffle(W& next_word, std::span<T> items) {
    for (std::size_t i = items.size() > 0 ? items.size() - 1 : 0; i > 0; --i) {
        const auto j = static_cast<std::size_t>(int_in_range(next_word, 0, static_cast<std::int64_t>(i)));
        using std::swap;
        swap(items[i], items[j]);
    }
}

// A word source from getentropy. Throws std::runtime_error if getentropy fails.
std::uint32_t secure_word();

// Returns nbytes secure random bytes as 2 * nbytes lowercase hexadecimal
// digits. Throws std::runtime_error if getentropy fails.
std::string token_hex(std::size_t nbytes);

}  // namespace random_numbers

#endif
