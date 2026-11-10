// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

#define _DEFAULT_SOURCE

#include "random_numbers.h"

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#if defined(__APPLE__)
#include <sys/random.h> // macOS declares getentropy here, not in <unistd.h>.
#endif

#define WORDS (UINT64_C(1) << 32)

// POSIX puts the limit of one call to getentropy in GETENTROPY_MAX. macOS does
// not define it, and its manual gives 256 bytes.
#ifndef GETENTROPY_MAX
#define GETENTROPY_MAX 256
#endif

bool int_in_range(random_words *next_word, void *ctx, int64_t lo, int64_t hi, int64_t *out) {
    // The unsigned difference is exact, also when hi - lo overflows int64_t.
    uint64_t span = (uint64_t)hi - (uint64_t)lo;
    if (hi < lo || span >= WORDS) {
        return false;
    }
    uint64_t n = span + 1;
    uint64_t least = WORDS % n;
    for (;;) {
        uint64_t word = next_word(ctx);
        if (word >= least) {
            // The sum is at most hi, so it fits. The unsigned addition avoids
            // the overflow of lo + (int64_t)r when r > INT64_MAX - lo.
            *out = (int64_t)((uint64_t)lo + word % n);
            return true;
        }
    }
}

static void swap_bytes(unsigned char *a, unsigned char *b, size_t size) {
    for (size_t k = 0; k < size; k++) {
        unsigned char t = a[k];
        a[k] = b[k];
        b[k] = t;
    }
}

bool shuffle(random_words *next_word, void *ctx, void *items, size_t count, size_t size) {
    if (count > WORDS) {
        return false;
    }
    unsigned char *base = items;
    for (size_t i = count > 0 ? count - 1 : 0; i > 0; i--) {
        int64_t j;
        int_in_range(next_word, ctx, 0, (int64_t)i, &j);
        if ((size_t)j != i) {
            swap_bytes(base + i * size, base + (size_t)j * size, size);
        }
    }
    return true;
}

uint32_t splitmix64_word(void *ctx) {
    struct splitmix64 *g = ctx;
    uint64_t z = (g->state += UINT64_C(0x9e3779b97f4a7c15));
    z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
    z ^= z >> 31;
    return (uint32_t)(z >> 32);
}

uint32_t secure_word(void *ctx) {
    (void)ctx;
    uint32_t w;
    if (getentropy(&w, sizeof w) != 0) {
        abort();
    }
    return w;
}

bool token_hex(size_t nbytes, char *out, size_t size) {
    static const char digits[] = "0123456789abcdef";
    if (nbytes > (SIZE_MAX - 1) / 2 || size < 2 * nbytes + 1) {
        return false;
    }
    unsigned char chunk[GETENTROPY_MAX];
    size_t done = 0;
    while (done < nbytes) {
        size_t len = nbytes - done < sizeof chunk ? nbytes - done : sizeof chunk;
        if (getentropy(chunk, len) != 0) {
            return false;
        }
        for (size_t k = 0; k < len; k++) {
            out[2 * (done + k)] = digits[chunk[k] >> 4];
            out[2 * (done + k) + 1] = digits[chunk[k] & 0xf];
        }
        done += len;
    }
    out[2 * nbytes] = '\0';
    return true;
}
