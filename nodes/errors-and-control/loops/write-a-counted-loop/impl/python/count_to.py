# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-counted-loop/.

def count_to(n: int) -> list[int]:
    """Return the list [1, 2, ..., n]."""
    out = []
    # range stops before its end, so the end is n + 1.
    for i in range(1, n + 1):
        out.append(i)
    return out
