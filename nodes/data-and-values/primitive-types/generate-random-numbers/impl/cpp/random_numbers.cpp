// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

#include "random_numbers.hpp"

#include <algorithm>
#include <array>
#include <climits>
#include <unistd.h>
#if defined(__APPLE__)
#include <sys/random.h>  // macOS declares getentropy here, not in <unistd.h>.
#endif

// POSIX puts the limit of one call to getentropy in GETENTROPY_MAX. macOS does
// not define it, and its manual gives 256 bytes.
#ifndef GETENTROPY_MAX
#define GETENTROPY_MAX 256
#endif

namespace random_numbers {

std::uint32_t secure_word() {
    std::uint32_t w;
    if (getentropy(&w, sizeof w) != 0) {
        throw std::runtime_error("getentropy failed");
    }
    return w;
}

std::string token_hex(std::size_t nbytes) {
    static constexpr char digits[] = "0123456789abcdef";
    std::string out;
    out.reserve(2 * nbytes);
    std::array<unsigned char, GETENTROPY_MAX> chunk;
    for (std::size_t done = 0; done < nbytes;) {
        const std::size_t len = std::min(nbytes - done, chunk.size());
        if (getentropy(chunk.data(), len) != 0) {
            throw std::runtime_error("getentropy failed");
        }
        for (std::size_t k = 0; k < len; ++k) {
            out += digits[chunk[k] >> 4];
            out += digits[chunk[k] & 0xf];
        }
        done += len;
    }
    return out;
}

}  // namespace random_numbers
