// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

#include "utf8.hpp"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

static int failures = 0;

static void check_encode(std::string_view name, std::uint32_t cp, const std::vector<std::uint8_t> &want) {
    if (encode_utf8(cp) != want) {
        std::cerr << "encode " << name << " fails\n";
        failures++;
    }
}

static void check_decode(std::string_view name, const std::vector<std::uint8_t> &b, long want) {
    if (decode_utf8(b) != want) {
        std::cerr << "decode " << name << " fails\n";
        failures++;
    }
}

// Each byte is a number from 0 to 255. A decode result of -1 means that the bytes are not one well-formed sequence.
int main() {
    check_encode("ascii-a", 0x41, {65});
    check_encode("euro", 0x20AC, {226, 130, 172});
    check_encode("emoji", 0x1F642, {240, 159, 153, 130});
    check_encode("nul", 0x0, {0});
    check_encode("one-byte-max", 0x7F, {127});
    check_encode("two-byte-min", 0x80, {194, 128});
    check_encode("two-byte", 0xE9, {195, 169});
    check_encode("two-byte-max", 0x7FF, {223, 191});
    check_encode("three-byte-min", 0x800, {224, 160, 128});
    check_encode("three-byte-max", 0xFFFF, {239, 191, 191});
    check_encode("four-byte-min", 0x10000, {240, 144, 128, 128});
    check_encode("max", 0x10FFFF, {244, 143, 191, 191});
    check_decode("ascii-a", {65}, 65);
    check_decode("e-acute", {195, 169}, 233);
    check_decode("emoji", {240, 159, 153, 130}, 128578);
    check_decode("max", {244, 143, 191, 191}, 1114111);
    check_decode("replacement", {239, 191, 189}, 65533);
    check_decode("empty", {}, -1);
    check_decode("stray-continuation", {128}, -1);
    check_decode("truncated", {195}, -1);
    check_decode("bad-continuation", {195, 40}, -1);
    check_decode("overlong-two", {192, 129}, -1);
    check_decode("overlong-three", {224, 128, 128}, -1);
    check_decode("surrogate", {237, 160, 128}, -1);
    check_decode("above-max", {244, 144, 128, 128}, -1);
    check_decode("two-sequences", {65, 66}, -1);
    if (failures > 0) {
        return 1;
    }
    std::cout << "all cases pass\n";
    return 0;
}
