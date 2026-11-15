# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-while-loop/.

def halvings(n: int) -> list[int]:
    """Return n, n // 2, n // 4, ... while the value is greater than 0."""
    out = []
    while n > 0:
        out.append(n)
        # n / 2 would make a float, and the loop would run about 1000 more times.
        n //= 2
    return out
