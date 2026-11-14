# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-word-count/.

"""Counts the lines, words and bytes of a UTF-8 file, as wc does."""

import codecs
from os import PathLike

CHUNK = 64 * 1024

# The 25 code points that have the Unicode property White_Space (PropList.txt).
WHITE_SPACE = frozenset(
    map(chr, [*range(0x09, 0x0E), 0x20, 0x85, 0xA0, 0x1680, *range(0x2000, 0x200B), 0x2028, 0x2029, 0x202F, 0x205F, 0x3000])
)


def word_count(path: str | PathLike[str]) -> tuple[int, int, int]:
    """Returns (lines, words, bytes) for the UTF-8 file at path.

    lines is the number of LF bytes. A word is a run of one or more code points
    that are not White_Space. Raises UnicodeDecodeError if the bytes are not
    valid UTF-8, and OSError, for example FileNotFoundError, if the file cannot
    be read.
    """
    decoder = codecs.getincrementaldecoder("utf-8")(errors="strict")
    lines = words = size = 0
    in_word = False

    def count(text: str) -> None:
        nonlocal lines, words, in_word
        for ch in text:
            if ch in WHITE_SPACE:
                in_word = False
                lines += ch == "\n"
            elif not in_word:
                in_word = True
                words += 1

    with open(path, "rb") as f:
        while chunk := f.read(CHUNK):
            size += len(chunk)
            # The decoder keeps a sequence that the chunk cuts until the next chunk.
            count(decoder.decode(chunk))
    count(decoder.decode(b"", final=True))  # raises if the file ends inside a sequence
    return lines, words, size
