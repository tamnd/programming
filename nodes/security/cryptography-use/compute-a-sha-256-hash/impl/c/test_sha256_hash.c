// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

#define _POSIX_C_SOURCE 200809L

#include "sha256_hash.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// A message is the bytes of a literal, or len copies of the byte fill.
#define S(s) s, sizeof(s) - 1, 0
#define REP(fill, n) NULL, n, fill

struct test_case {
    const char *name;
    const char *data; // NULL: len copies of fill
    size_t len;
    char fill;
    const char *want;
};

static const struct test_case text_cases[] = {
    {"abc", S("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"},
    {"two-block-56", S("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"),
     "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1"},
    {"fox", S("The quick brown fox jumps over the lazy dog"),
     "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592"},
    {"empty", S(""), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
    {"a-55", REP('a', 55), "9f4390f8d30c2dd92ec9f095b65e2b9ae9b0a925a5258e241c9f1e910f734318"},
    {"a-56", REP('a', 56), "b35439a4ac6f0948b6d6f9e3c6af0f5f590ce20f1bde7090ef7970686ec6738a"},
    {"a-63", REP('a', 63), "7d3e74a05d7db15bce4ad9ec0658ea98e3f06eeecf16b4c6fff2da457ddc2f34"},
    {"a-64", REP('a', 64), "ffe054fe7ae0cb6dc65c3af9b61d5209f439851db43d0ba5997337df154668eb"},
    {"multi-block-112",
     S("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
       "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"),
     "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1"},
    {"non-ascii", S("na\xc3\xafve caf\xc3\xa9 \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e \xf0\x9f\x99\x82"),
     "9c53e090acd67abd65c64ee431a36bdc36c3fd86341f5814a94d1b901ca9ed8d"},
    {"nfc-cafe", S("caf\xc3\xa9"), "850f7dc43910ff890f8879c0ed26fe697c93a067ad93a7d50f466a7028a9bf4e"},
    {"nfd-cafe", S("cafe\xcc\x81"), "81ef060bcd98adc7824eb5c1ada83c32491b16018e11e79f00ab9d09e04b015a"},
    {"trailing-newline", S("abc\n"), "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"},
};

// From cases/files.jsonl.
static const struct test_case file_cases[] = {
    {"abc-file", S("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"},
    {"abc-newline-file", S("abc\n"), "edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb"},
    {"abc-crlf-file", S("abc\r\n"), "552bab6864c7a7b69a502ed1854b9245c0e1a30f008aaa0b281da62585fdb025"},
    {"empty-file", S(""), "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"},
    {"binary-file", S("\x00\x01\xff"), "26a66b061e8f48f39927c312f25293959729eee95978e2892d49d3512a5cc092"},
    // One million repetitions of "a": more than one chunk.
    {"million-a-file", REP('a', 1000000), "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0"},
};

// The SHA-256 Test Data of the NIST example values, SHA2_Additional.pdf.
static const struct test_case nist_cases[] = {
    {"nist-1", S("\xbd"), "68325720aabd7c82f30f554b313d0570c95accbb7dc4b5aae11204c08ffe732b"},
    {"nist-2", S("\xc9\x8c\x8e\x55"), "7abc22c0ae5af26ce93dbb94433a0e0b2e119d014f8e7f65bd56c61ccccd9504"},
    {"nist-3", REP(0, 55), "02779466cdec163811d078815c633f21901413081449002f24aa3e80f0b88ef7"},
    {"nist-4", REP(0, 56), "d4817aa5497628e7c77e6b606107042bbba3130888c5f47a375e6179be789fbb"},
    {"nist-5", REP(0, 57), "65a16cb7861335d5ace3c60718b5052e44660726da4cd13bb745381b235a1785"},
    {"nist-6", REP(0, 64), "f5a5fd42d16a20302798ef6ed309979b43003d2320d9f0e8ea9831a92759fb4b"},
    {"nist-7", REP(0, 1000), "541b3e9daa09b20bf85fa273e5cbd3e80185aa4ec298e765db87742b70138a53"},
    {"nist-8", REP('A', 1000), "c2e686823489ced2017f6059b8b239318b6364f6dcd835d0a519105a1eadd6e4"},
    {"nist-9", REP('U', 1005), "f4d62ddec0f3dd90ea1380fa16a5ff8dc4c54b21740650f24afc4120903552b0"},
    {"nist-10", REP(0, 1000000), "d29751f2649b32ff572b5e0a9f541ea660a50f94ff0beedfb0b692b924cc8025"},
};

#define COUNT(a) (sizeof(a) / sizeof((a)[0]))

// Returns the bytes of the message of c in a new buffer.
static char *message(const struct test_case *c) {
    char *buf = malloc(c->len + 1);
    if (buf == NULL) {
        perror("malloc");
        exit(1);
    }
    if (c->data != NULL) {
        memcpy(buf, c->data, c->len);
    } else {
        memset(buf, c->fill, c->len);
    }
    return buf;
}

static int check(const char *name, const char *got, const char *want) {
    if (strcmp(got, want) != 0) {
        printf("FAIL %s: got %s, want %s\n", name, got, want);
        return 1;
    }
    return 0;
}

int main(void) {
    int failures = 0;
    char out[65];

    for (size_t i = 0; i < COUNT(text_cases); i++) {
        char *msg = message(&text_cases[i]);
        sha256_hex(msg, text_cases[i].len, out);
        failures += check(text_cases[i].name, out, text_cases[i].want);
        free(msg);
    }
    for (size_t i = 0; i < COUNT(nist_cases); i++) {
        char *msg = message(&nist_cases[i]);
        sha256_hex(msg, nist_cases[i].len, out);
        failures += check(nist_cases[i].name, out, nist_cases[i].want);
        free(msg);
    }

    char dir[64];
    snprintf(dir, sizeof dir, "/tmp/sha256-%ld", (long)getpid());
    if (mkdir(dir, 0700) != 0) {
        perror(dir);
        return 1;
    }
    char path[128];
    snprintf(path, sizeof path, "%s/in.bin", dir);
    for (size_t i = 0; i < COUNT(file_cases); i++) {
        const struct test_case *c = &file_cases[i];
        char *msg = message(c);
        FILE *f = fopen(path, "wb");
        if (f == NULL || fwrite(msg, 1, c->len, f) != c->len || fclose(f) != 0) {
            perror(path);
            return 1;
        }
        free(msg);
        if (sha256_file(path, out) != 0) {
            printf("FAIL %s: %s\n", c->name, strerror(errno));
            failures++;
        } else {
            failures += check(c->name, out, c->want);
        }
    }
    remove(path);

    snprintf(path, sizeof path, "%s/missing.txt", dir);
    errno = 0;
    if (sha256_file(path, out) != -1 || errno != ENOENT) {
        printf("FAIL missing-file: no ENOENT\n");
        failures++;
    }
    rmdir(dir);

    // The message in parts of 1, 2, ..., 130 bytes gives the same digest as in one part.
    unsigned char data[1000];
    for (size_t i = 0; i < sizeof data; i++) {
        data[i] = (unsigned char)(i % 251);
    }
    char want[65];
    sha256_hex(data, sizeof data, want);
    for (size_t size = 1; size <= 130; size++) {
        struct sha256 h;
        unsigned char digest[32];
        sha256_init(&h);
        for (size_t i = 0; i < sizeof data; i += size) {
            sha256_update(&h, data + i, sizeof data - i < size ? sizeof data - i : size);
        }
        sha256_final(&h, digest);
        for (int i = 0; i < 32; i++) {
            snprintf(out + 2 * i, 3, "%02x", digest[i]);
        }
        if (strcmp(out, want) != 0) {
            printf("FAIL parts of %zu\n", size);
            failures++;
        }
    }

    if (failures > 0) {
        printf("%d failures\n", failures);
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
