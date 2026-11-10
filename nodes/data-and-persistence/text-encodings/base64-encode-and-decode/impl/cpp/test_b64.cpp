// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "b64.hpp"

namespace {

int failures = 0;

void fail(std::string_view name) {
    std::fprintf(stderr, "case %.*s failed\n", static_cast<int>(name.size()), name.data());
    failures++;
}

b64::Alphabet alphabet(std::string_view name) {
    return name == "url" ? b64::Alphabet::url : b64::Alphabet::standard;
}

std::vector<std::uint8_t> from_hex(std::string_view hex) {
    std::vector<std::uint8_t> data;
    for (std::size_t i = 0; i + 1 < hex.size(); i += 2) {
        data.push_back(static_cast<std::uint8_t>(std::stoi(std::string(hex.substr(i, 2)), nullptr, 16)));
    }
    return data;
}

void check_encode(std::string_view name, std::string_view hex, std::string_view a,
                  std::string_view want) {
    const auto data = from_hex(hex);
    const std::string text = b64::encode(data, alphabet(a));
    if (text != want) {
        fail(name);
        return;
    }
    try {
        if (b64::decode(text, alphabet(a)) != data) {
            fail(name);
        }
    } catch (const std::invalid_argument &) {
        fail(name);
    }
}

void check_decode(std::string_view name, std::string_view text, std::string_view a,
                  std::string_view want) {
    try {
        if (b64::decode(text, alphabet(a)) != from_hex(want)) {
            fail(name);
        }
    } catch (const std::invalid_argument &) {
        fail(name);
    }
}

void check_invalid(std::string_view name, std::string_view text, std::string_view a) {
    try {
        b64::decode(text, alphabet(a));
        fail(name);
    } catch (const std::invalid_argument &) {
    }
}

}  // namespace

int main() {
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
