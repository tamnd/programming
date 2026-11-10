// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

// SHA-256 as FIPS 180-4 defines it. The C standard library has no SHA-256.

#include "sha256_hash.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

// FIPS 180-4, 4.2.2: the first 32 bits of the fractional parts of the cube
// roots of the first 64 primes.
static const uint32_t K[64] = {
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
static const uint32_t H0[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
};

static uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }

// FIPS 180-4, 6.2.2: the hash computation for one 512-bit block.
static void compress(uint32_t state[8], const unsigned char block[64]) {
    uint32_t w[64];
    for (int t = 0; t < 16; t++) {
        const unsigned char *p = block + 4 * t;
        w[t] = (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | p[3];
    }
    for (int t = 16; t < 64; t++) {
        uint32_t s0 = rotr(w[t - 15], 7) ^ rotr(w[t - 15], 18) ^ (w[t - 15] >> 3);
        uint32_t s1 = rotr(w[t - 2], 17) ^ rotr(w[t - 2], 19) ^ (w[t - 2] >> 10);
        w[t] = s1 + w[t - 7] + s0 + w[t - 16];
    }
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t e = state[4], f = state[5], g = state[6], h = state[7];
    for (int t = 0; t < 64; t++) {
        uint32_t t1 = h + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) + ((e & f) ^ (~e & g)) + K[t] + w[t];
        uint32_t t2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

void sha256_init(struct sha256 *h) {
    memcpy(h->state, H0, sizeof H0);
    h->used = 0;
    h->len = 0;
}

void sha256_update(struct sha256 *h, const void *data, size_t len) {
    const unsigned char *p = data;
    h->len += len;
    while (len > 0) {
        size_t n = 64 - h->used;
        if (n > len) {
            n = len;
        }
        memcpy(h->block + h->used, p, n);
        h->used += n;
        p += n;
        len -= n;
        if (h->used == 64) {
            compress(h->state, h->block);
            h->used = 0;
        }
    }
}

void sha256_final(struct sha256 *h, unsigned char digest[32]) {
    // FIPS 180-4, 5.1.1: a 1 bit, then zero bits, then the length in bits as
    // a 64-bit big-endian number, to a multiple of 512 bits.
    uint64_t bits = h->len * 8;
    h->block[h->used++] = 0x80;
    if (h->used > 56) {
        // No room for the length: it goes in one more block.
        memset(h->block + h->used, 0, 64 - h->used);
        compress(h->state, h->block);
        h->used = 0;
    }
    memset(h->block + h->used, 0, 56 - h->used);
    for (int i = 0; i < 8; i++) {
        h->block[56 + i] = (unsigned char)(bits >> (56 - 8 * i));
    }
    compress(h->state, h->block);
    for (int i = 0; i < 8; i++) {
        digest[4 * i] = (unsigned char)(h->state[i] >> 24);
        digest[4 * i + 1] = (unsigned char)(h->state[i] >> 16);
        digest[4 * i + 2] = (unsigned char)(h->state[i] >> 8);
        digest[4 * i + 3] = (unsigned char)h->state[i];
    }
}

static void to_hex(const unsigned char digest[32], char out[65]) {
    static const char digits[] = "0123456789abcdef";
    for (int i = 0; i < 32; i++) {
        out[2 * i] = digits[digest[i] >> 4];
        out[2 * i + 1] = digits[digest[i] & 0x0f];
    }
    out[64] = '\0';
}

void sha256_hex(const void *data, size_t len, char out[65]) {
    struct sha256 h;
    unsigned char digest[32];
    sha256_init(&h);
    sha256_update(&h, data, len);
    sha256_final(&h, digest);
    to_hex(digest, out);
}

int sha256_file(const char *path, char out[65]) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        return -1;
    }
    struct sha256 h;
    unsigned char buf[8192];
    size_t n;
    sha256_init(&h);
    // The file is read in chunks, so a large file does not fill the memory.
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
        sha256_update(&h, buf, n);
    }
    if (ferror(f)) {
        int err = errno;
        fclose(f);
        errno = err;
        return -1;
    }
    fclose(f);
    unsigned char digest[32];
    sha256_final(&h, digest);
    to_hex(digest, out);
    return 0;
}
