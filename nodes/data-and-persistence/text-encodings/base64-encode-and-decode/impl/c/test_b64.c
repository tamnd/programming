// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

#include "b64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void fail(const char *name) {
    fprintf(stderr, "case %s failed\n", name);
    failures++;
}

static enum b64_alphabet alphabet(const char *name) {
    return strcmp(name, "url") == 0 ? B64_URL : B64_STANDARD;
}

// Writes the bytes of hex to out and returns their count.
static size_t from_hex(const char *hex, unsigned char *out) {
    size_t n = strlen(hex) / 2;
    for (size_t i = 0; i < n; i++) {
        unsigned int b = 0;
        sscanf(hex + 2 * i, "%2x", &b);
        out[i] = (unsigned char)b;
    }
    return n;
}

// Checks that the len bytes at data are the bytes of hex.
static int same(const unsigned char *data, size_t len, const char *hex) {
    unsigned char want[64];
    return len == from_hex(hex, want) && (len == 0 || memcmp(data, want, len) == 0);
}

static void check_encode(const char *name, const char *hex, const char *a, const char *want) {
    unsigned char data[64];
    size_t len = from_hex(hex, data);
    char *text = b64_encode(data, len, alphabet(a));
    if (text == NULL || strcmp(text, want) != 0) {
        fail(name);
        free(text);
        return;
    }
    unsigned char *back = NULL;
    size_t back_len = 0;
    if (b64_decode(text, alphabet(a), &back, &back_len) != B64_OK || !same(back, back_len, hex)) {
        fail(name);
    }
    free(back);
    free(text);
}

static void check_decode(const char *name, const char *text, const char *a, const char *want) {
    unsigned char *data = NULL;
    size_t len = 0;
    if (b64_decode(text, alphabet(a), &data, &len) != B64_OK || !same(data, len, want)) {
        fail(name);
    }
    free(data);
}

static void check_invalid(const char *name, const char *text, const char *a) {
    unsigned char *data = NULL;
    size_t len = 0;
    if (b64_decode(text, alphabet(a), &data, &len) != B64_INVALID || data != NULL) {
        fail(name);
    }
    free(data);
}

int main(void) {
    // The bytes are written in hexadecimal.
    check_encode("rfc-f", "66", "standard", "Zg==");
    check_encode("rfc-fo", "666f", "standard", "Zm8=");
    check_encode("rfc-foo", "666f6f", "standard", "Zm9v");
    check_encode("rfc-foob", "666f6f62", "standard", "Zm9vYg==");
    check_encode("rfc-fooba", "666f6f6261", "standard", "Zm9vYmE=");
    check_encode("rfc-foobar", "666f6f626172", "standard", "Zm9vYmFy");
    check_encode("hello-standard", "48656c6c6f2c20776f726c6421", "standard", "SGVsbG8sIHdvcmxkIQ==");
    check_encode("hello-url", "48656c6c6f2c20776f726c6421", "url", "SGVsbG8sIHdvcmxkIQ");
    check_encode("empty-standard", "", "standard", "");
    check_encode("empty-url", "", "url", "");
    check_encode("two-pad-chars", "00", "standard", "AA==");
    check_encode("one-pad-char", "0000", "standard", "AAA=");
    check_encode("zero-bytes", "000000", "standard", "AAAA");
    check_encode("plus-and-slash", "fbff", "standard", "+/8=");
    check_encode("minus-and-underscore", "fbff", "url", "-_8");
    check_encode("all-62-standard", "fbefbe", "standard", "++++");
    check_encode("all-62-url", "fbefbe", "url", "----");
    check_encode("all-63-standard", "ffffff", "standard", "////");
    check_encode("all-63-url", "ffffff", "url", "____");
    check_encode("url-one-byte", "66", "url", "Zg");
    check_encode("url-two-bytes", "666f", "url", "Zm8");
    check_encode("non-ascii-standard", "c3a9e282acf09f9982", "standard", "w6nigqzwn5mC");
    check_encode("non-ascii-url", "c3a9e282acf09f9982", "url", "w6nigqzwn5mC");

    check_decode("rfc-empty", "", "standard", "");
    check_decode("rfc-f", "Zg==", "standard", "66");
    check_decode("rfc-fo", "Zm8=", "standard", "666f");
    check_decode("rfc-foo", "Zm9v", "standard", "666f6f");
    check_decode("rfc-foob", "Zm9vYg==", "standard", "666f6f62");
    check_decode("rfc-fooba", "Zm9vYmE=", "standard", "666f6f6261");
    check_decode("rfc-foobar", "Zm9vYmFy", "standard", "666f6f626172");
    check_decode("empty-url", "", "url", "");
    check_decode("url-foob", "Zm9vYg", "url", "666f6f62");
    check_decode("url-fooba", "Zm9vYmE", "url", "666f6f6261");
    check_decode("url-foobar", "Zm9vYmFy", "url", "666f6f626172");
    check_decode("plus-and-slash", "+/8=", "standard", "fbff");
    check_decode("minus-and-underscore", "-_8", "url", "fbff");
    check_decode("all-63-url", "____", "url", "ffffff");
    check_decode("zero-bytes", "AAAA", "standard", "000000");
    check_decode("non-ascii-standard", "w6nigqzwn5mC", "standard", "c3a9e282acf09f9982");
    check_decode("non-ascii-url", "w6nigqzwn5mC", "url", "c3a9e282acf09f9982");

    check_invalid("plus-in-url", "+/8", "url");
    check_invalid("minus-in-standard", "-_8=", "standard");
    check_invalid("bang", "Zm9v!", "standard");
    check_invalid("non-ascii-char", "Zm9vé===", "standard");
    check_invalid("space", "Zm9v YmFy", "standard");
    check_invalid("line-feed", "Zm9v\nYmFy", "standard");
    check_invalid("trailing-crlf", "Zm9vYmFy\r\n", "standard");
    check_invalid("line-feed-url", "Zm9v\nYmFy", "url");
    check_invalid("missing-padding", "Zm8", "standard");
    check_invalid("missing-one-pad", "Zg=", "standard");
    check_invalid("excess-padding", "Zm9v=", "standard");
    check_invalid("padding-in-middle", "Zm=9", "standard");
    check_invalid("three-pads", "Z===", "standard");
    check_invalid("only-padding", "====", "standard");
    check_invalid("padding-then-data", "Zg==Zm8=", "standard");
    check_invalid("padding-in-url", "Zm8=", "url");
    check_invalid("length-1-mod-4-standard", "Zm9vY", "standard");
    check_invalid("length-1-mod-4-url", "Zm9vY", "url");
    check_invalid("one-char-url", "Z", "url");
    check_invalid("pad-bits-one-byte", "QR==", "standard");
    check_invalid("pad-bits-two-bytes", "Zm9=", "standard");
    check_invalid("pad-bits-url", "Zm9", "url");

    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
