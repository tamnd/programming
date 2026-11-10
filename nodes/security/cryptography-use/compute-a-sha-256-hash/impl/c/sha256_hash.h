// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

#ifndef SHA256_HASH_H
#define SHA256_HASH_H

#include <stddef.h>
#include <stdint.h>

// A SHA-256 computation that takes the message in parts (FIPS 180-4).
struct sha256 {
    uint32_t state[8];
    unsigned char block[64];
    size_t used;
    uint64_t len; // the length of the message in bytes
};

void sha256_init(struct sha256 *h);
void sha256_update(struct sha256 *h, const void *data, size_t len);
void sha256_final(struct sha256 *h, unsigned char digest[32]);

// Writes the SHA-256 of the len bytes at data to out, as 64 lowercase hex
// digits and a NUL. For text, pass its UTF-8 bytes.
void sha256_hex(const void *data, size_t len, char out[65]);

// Writes the SHA-256 of the bytes of the file at path to out, as in
// sha256_hex. It returns 0, or -1 with errno set.
int sha256_file(const char *path, char out[65]);

#endif
