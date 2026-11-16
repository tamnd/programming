# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/overload-an-operator/.

"""A 2D vector with the operators + and ==."""


class Vec2:
    __slots__ = ("_x", "_y")

    def __init__(self, x: int, y: int) -> None:
        self._x, self._y = x, y

    @property
    def x(self) -> int:
        return self._x

    @property
    def y(self) -> int:
        return self._y

    def __add__(self, other: object) -> "Vec2":
        if not isinstance(other, Vec2):
            return NotImplemented
        return Vec2(self._x + other._x, self._y + other._y)

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Vec2):
            return NotImplemented
        return self._x == other._x and self._y == other._y

    def __hash__(self) -> int:
        return hash((self._x, self._y))

    def __repr__(self) -> str:
        return f"Vec2({self._x}, {self._y})"


def vector_sum(xs: list[int], ys: list[int]) -> list[int]:
    if len(xs) != len(ys):
        raise ValueError("the lists must have the same length")
    # sum() starts from the zero vector and applies + to each item.
    total = sum((Vec2(x, y) for x, y in zip(xs, ys)), Vec2(0, 0))
    return [total.x, total.y]
