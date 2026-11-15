# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-class/.


class Rectangle:
    """A rectangle with a width and a height."""

    def __init__(self, width: int, height: int) -> None:
        # The constructor sets the fields of the new instance on self.
        self.width = width
        self.height = height

    def area(self) -> int:
        return self.width * self.height

    def perimeter(self) -> int:
        return 2 * (self.width + self.height)

    def scale(self, factor: int) -> None:
        """Multiply both sides by factor. This changes the instance."""
        self.width *= factor
        self.height *= factor

    def __repr__(self) -> str:
        return f"Rectangle({self.width}, {self.height})"


def rectangle_report(width: int, height: int, factor: int) -> list[int]:
    r = Rectangle(width, height)
    before = [r.area(), r.perimeter()]
    r.scale(factor)
    return before + [r.area(), r.perimeter()]
