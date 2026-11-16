# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

"""A Color with a display form for users and a debug form for programmers."""


class Color:
    __slots__ = ("_r", "_g", "_b")

    def __init__(self, r: int, g: int, b: int) -> None:
        for v in (r, g, b):
            if not 0 <= v <= 255:
                raise ValueError(f"not a channel value: {v}")
        self._r, self._g, self._b = r, g, b

    def __str__(self) -> str:
        # The display form: print, str() and f-strings call it.
        return f"#{self._r:02X}{self._g:02X}{self._b:02X}"

    def __repr__(self) -> str:
        # The debug form: repr(), the prompt and the items of a list call it.
        return f"Color(r={self._r}, g={self._g}, b={self._b})"


def format_colors(reds: list[int], greens: list[int], blues: list[int]) -> list[str]:
    if not len(reds) == len(greens) == len(blues):
        raise ValueError("the lists must have the same length")
    return [str(Color(r, g, b)) for r, g, b in zip(reds, greens, blues)]
