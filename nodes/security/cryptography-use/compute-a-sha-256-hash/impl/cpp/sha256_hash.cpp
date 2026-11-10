// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

// SHA-256 as FIPS 180-4 defines it. The C++ standard library has no SHA-256.

#include "sha256_hash.hpp"

#include <algorithm>
#include <bit>
#include <fstream>

namespace {

// FIPS 180-4, 4.2.2: the first 32 bits of the fractional parts of the cube
// roots of the first 64 primes.
constexpr std::array<std::uint32_t, 64> K = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

// FIPS 180-4, 5.3.3: the initial hash value.
constexpr std::array<std::uint32_t, 8> H0 = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
};

// FIPS 180-4, 6.2.2: the hash computation for one 512-bit block.
void compress(std::array<std::uint32_t, 8>& state, const std::array<std::uint8_t, 64>& block) {
    using std::rotr;
    std::array<std::uint32_t, 64> w{};
    for (std::size_t t = 0; t < 16; ++t) {
        w[t] = std::uint32_t{block[4 * t]} << 24 | std::uint32_t{block[4 * t + 1]} << 16 |
               std::uint32_t{block[4 * t + 2]} << 8 | std::uint32_t{block[4 * t + 3]};
    }
    for (std::size_t t = 16; t < 64; ++t) {
        std::uint32_t s0 = rotr(w[t - 15], 7) ^ rotr(w[t - 15], 18) ^ (w[t - 15] >> 3);
        std::uint32_t s1 = rotr(w[t - 2], 17) ^ rotr(w[t - 2], 19) ^ (w[t - 2] >> 10);
        w[t] = s1 + w[t - 7] + s0 + w[t - 16];
    }
    auto [a, b, c, d, e, f, g, h] = state;
    for (std::size_t t = 0; t < 64; ++t) {
        std::uint32_t t1 = h + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) + ((e & f) ^ (~e & g)) + K[t] + w[t];
        std::uint32_t t2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    const std::array<std::uint32_t, 8> v = {a, b, c, d, e, f, g, h};
    for (std::size_t i = 0; i < 8; ++i) {
        state[i] += v[i];
    }
}

}  // namespace

Sha256::Sha256() : state_(H0) {}

void Sha256::update(std::span<const std::byte> data) {
    len_ += data.size();
    while (!data.empty()) {
        std::size_t n = std::min(64 - used_, data.size());
        for (std::size_t i = 0; i < n; ++i) {
            block_[used_ + i] = static_cast<std::uint8_t>(data[i]);
        }
        used_ += n;
        data = data.subspan(n);
        if (used_ == 64) {
            compress(state_, block_);
            used_ = 0;
        }
    }
}

Sha256::Digest Sha256::finalize() {
    // FIPS 180-4, 5.1.1: a 1 bit, then zero bits, then the length in bits as
    // a 64-bit big-endian number, to a multiple of 512 bits.
    const std::uint64_t bits = len_ * 8;
    block_[used_++] = 0x80;
    if (used_ > 56) {
        // No room for the length: it goes in one more block.
        std::fill(block_.begin() + used_, block_.end(), 0);
        compress(state_, block_);
        used_ = 0;
    }
    std::fill(block_.begin() + used_, block_.begin() + 56, 0);
    for (std::size_t i = 0; i < 8; ++i) {
        block_[56 + i] = static_cast<std::uint8_t>(bits >> (56 - 8 * i));
    }
    compress(state_, block_);
    Digest out{};
    for (std::size_t i = 0; i < 8; ++i) {
        out[4 * i] = static_cast<std::uint8_t>(state_[i] >> 24);
        out[4 * i + 1] = static_cast<std::uint8_t>(state_[i] >> 16);
        out[4 * i + 2] = static_cast<std::uint8_t>(state_[i] >> 8);
        out[4 * i + 3] = static_cast<std::uint8_t>(state_[i]);
    }
    return out;
}

std::string to_hex(const Sha256::Digest& digest) {
    constexpr std::string_view digits = "0123456789abcdef";
    std::string out;
    out.reserve(64);
    for (std::uint8_t b : digest) {
        out += digits[b >> 4];
        out += digits[b & 0x0f];
    }
    return out;
}

std::string sha256_text(std::string_view text) {
    Sha256 h;
    h.update(text);
    return to_hex(h.finalize());
}

std::expected<std::string, HashError> sha256_file(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return std::unexpected(HashError::cannot_open);
    }
    Sha256 h;
    std::array<char, 8192> buf;
    // The file is read in chunks, so a large file does not fill the memory.
    while (in.read(buf.data(), buf.size()) || in.gcount() > 0) {
        h.update(std::string_view(buf.data(), static_cast<std::size_t>(in.gcount())));
    }
    if (in.bad()) {
        return std::unexpected(HashError::read_failed);
    }
    return to_hex(h.finalize());
}
