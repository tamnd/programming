# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/stream-parse-a-large-json-file/.

import json
from collections.abc import Iterator
from typing import Any, TextIO

CHUNK = 1 << 16  # characters read at a time
SPACE = " \t\n\r"


class BadRecord(Exception):
    """An element of the array is not an object with a number member amount."""


def reject_constant(name: str) -> None:
    # json accepts NaN, Infinity and -Infinity unless this hook refuses them.
    raise ValueError(f"{name} is not a JSON value")


DECODER = json.JSONDecoder(parse_constant=reject_constant)


def elements(f: TextIO) -> Iterator[Any]:
    """Yields the elements of the JSON array in f, one at a time."""
    buf, pos, eof = "", 0, False

    def more() -> bool:
        # Drops the text before pos and reads the next chunk. Returns False at the end of the file.
        nonlocal buf, pos, eof
        chunk = f.read(CHUNK)
        buf, pos, eof = buf[pos:] + chunk, 0, not chunk
        return not eof

    def next_char() -> str:
        # Skips white space and returns the next character, or "" at the end of the file.
        nonlocal pos
        while True:
            while pos < len(buf) and buf[pos] in SPACE:
                pos += 1
            if pos < len(buf) or not more():
                return buf[pos:pos + 1]

    if next_char() != "[":
        raise ValueError("the text does not start with [")
    pos += 1
    if next_char() == "]":
        pos += 1
    else:
        while True:
            next_char()
            while True:
                try:
                    value, end = DECODER.raw_decode(buf, pos)
                    # A number at the end of the buffer can go on in the next chunk.
                    if end < len(buf) or eof:
                        break
                except json.JSONDecodeError:
                    if eof:
                        raise
                more()
            pos = end
            yield value
            c = next_char()
            pos += 1
            if c == "]":
                break
            if c != ",":
                raise ValueError("expected , or ] after an element")
    if next_char() != "":
        raise ValueError("data after the array")


def stream_sum(path: str) -> list[int]:
    """Returns the number of objects in the JSON array in the file, and the sum of their member amount."""
    count = total = 0
    # utf-8-sig drops a byte order mark at the start, which RFC 8259 allows a parser to ignore.
    with open(path, encoding="utf-8-sig") as f:
        for record in elements(f):
            amount = record.get("amount") if isinstance(record, dict) else None
            if not isinstance(amount, int) or isinstance(amount, bool):
                raise BadRecord(f"element {count} has no number member amount")
            count += 1
            total += amount
    return [count, total]
