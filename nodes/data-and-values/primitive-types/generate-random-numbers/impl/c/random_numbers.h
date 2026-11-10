// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H

#include <stddef.h>
#include <stdint.h>

// A word source returns a random integer from 0 to 2^32 - 1. ctx is its state.
typedef uint32_t random_words(void *ctx);

// Stores in *out an integer from lo to hi, each with equal probability, and
// returns true. Returns false if hi < lo or the range has more than 2^32
// values. A word below 2^32 % n is rejected, so that each remainder comes
// from the same number of words.
bool int_in_range(random_words *next_word, void *ctx, int64_t lo, int64_t hi, int64_t *out);

// Shuffles count items of size bytes each in place with the Fisher-Yates
// method. Returns false if count is more than 2^32.
bool shuffle(random_words *next_word, void *ctx, void *items, size_t count, size_t size);

// SplitMix64 (Steele, Lea and Flood), as in the reference code of Sebastiano
// Vigna. The same seed gives the same words.
struct splitmix64 {
    uint64_t state;
};

// A word source: the top 32 bits of the next output of a struct splitmix64.
uint32_t splitmix64_word(void *ctx);

// A word source from getentropy. It aborts if getentropy fails. ctx is unused.
uint32_t secure_word(void *ctx);

// Writes nbytes secure random bytes as 2 * nbytes lowercase hexadecimal
// digits and a null byte to out, and returns true. Returns false if size is
// less than 2 * nbytes + 1 or if getentropy fails.
bool token_hex(size_t nbytes, char *out, size_t size);

#endif
