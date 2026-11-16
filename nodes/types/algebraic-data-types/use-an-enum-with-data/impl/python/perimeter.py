# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-an-enum-with-data/.
from dataclasses import dataclass


@dataclass(frozen=True)
class Square:
    side: int


@dataclass(frozen=True)
class Rectangle:
    width: int
    height: int


@dataclass(frozen=True)
class Triangle:
    a: int
    b: int
    c: int


# A union of three classes: a value of Shape is exactly one of them.
type Shape = Square | Rectangle | Triangle


def make_shape(kind: str, sides: list[int]) -> Shape | None:
    """Builds a shape, or returns None when the data is not a valid shape."""
    if any(s <= 0 for s in sides):
        return None
    match kind, sides:
        case "square", [side]:
            return Square(side)
        case "rectangle", [width, height]:
            return Rectangle(width, height)
        case "triangle", [a, b, c] if a < b + c and b < a + c and c < a + b:
            return Triangle(a, b, c)
        case _:
            return None


def shape_perimeter(shape: Shape) -> int:
    match shape:
        case Square(side):
            return 4 * side
        case Rectangle(width, height):
            return 2 * (width + height)
        case Triangle(a, b, c):
            return a + b + c


def perimeter(kind: str, sides: list[int]) -> int | None:
    shape = make_shape(kind, sides)
    return None if shape is None else shape_perimeter(shape)
