# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

import pytest

from utf8 import decode_utf8, encode_utf8

# Each byte is a number from 0 to 255. A decode result of -1 means that the bytes are not one well-formed sequence.
ENCODE = {
    "ascii-a": (0x41, [65]),
    "euro": (0x20AC, [226, 130, 172]),
    "emoji": (0x1F642, [240, 159, 153, 130]),
    "nul": (0x0, [0]),
    "one-byte-max": (0x7F, [127]),
    "two-byte-min": (0x80, [194, 128]),
    "two-byte": (0xE9, [195, 169]),
    "two-byte-max": (0x7FF, [223, 191]),
    "three-byte-min": (0x800, [224, 160, 128]),
    "three-byte-max": (0xFFFF, [239, 191, 191]),
    "four-byte-min": (0x10000, [240, 144, 128, 128]),
    "max": (0x10FFFF, [244, 143, 191, 191]),
}

DECODE = {
    "ascii-a": ([65], 65),
    "e-acute": ([195, 169], 233),
    "emoji": ([240, 159, 153, 130], 128578),
    "max": ([244, 143, 191, 191], 1114111),
    "replacement": ([239, 191, 189], 65533),
    "empty": ([], -1),
    "stray-continuation": ([128], -1),
    "truncated": ([195], -1),
    "bad-continuation": ([195, 40], -1),
    "overlong-two": ([192, 129], -1),
    "overlong-three": ([224, 128, 128], -1),
    "surrogate": ([237, 160, 128], -1),
    "above-max": ([244, 144, 128, 128], -1),
    "two-sequences": ([65, 66], -1),
}


@pytest.mark.parametrize("name", ENCODE)
def test_encode_utf8(name):
    cp, want = ENCODE[name]
    assert encode_utf8(cp) == want


@pytest.mark.parametrize("name", DECODE)
def test_decode_utf8(name):
    b, want = DECODE[name]
    assert decode_utf8(b) == want
