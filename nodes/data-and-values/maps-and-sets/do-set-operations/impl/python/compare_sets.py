# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-set-operations/.


def compare_sets(
    a: list[str], b: list[str]
) -> tuple[list[str], list[str], list[str], list[str], bool]:
    """Return the union, intersection, difference and symmetric difference
    of the sets of a and b, each sorted, and whether set(a) is a subset of set(b)."""
    sa, sb = set(a), set(b)
    return (
        sorted(sa | sb),
        sorted(sa & sb),
        sorted(sa - sb),
        sorted(sa ^ sb),
        # <= is subset. < is proper subset, which is false for equal sets.
        sa <= sb,
    )
