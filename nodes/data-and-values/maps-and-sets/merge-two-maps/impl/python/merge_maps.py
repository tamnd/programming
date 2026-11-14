# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/merge-two-maps/.


def merge_maps(base: dict[str, int], override: dict[str, int]) -> list[tuple[str, int]]:
    """Return the entries of both dicts in key order; the value of override wins."""
    # | makes a new dict. The values of the right operand win.
    merged = base | override
    return sorted(merged.items())
