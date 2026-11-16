# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compose-functions/.

from collections.abc import Callable
from functools import reduce


def identity[T](x: T) -> T:
    """Return x."""
    return x


def compose[T](*functions: Callable[[T], T]) -> Callable[[T], T]:
    """Return the composition of the functions. The last one runs first."""
    return reduce(lambda f, g: lambda x: f(g(x)), functions, identity)


OPS: dict[str, Callable[[int], int]] = {
    "inc": lambda x: x + 1,
    "dec": lambda x: x - 1,
    "double": lambda x: 2 * x,
    "negate": lambda x: -x,
}


def compose_all(ops: list[str], x: int) -> int:
    """Compose the functions that ops names and apply the result to x."""
    return compose(*(OPS[name] for name in ops))(x)
