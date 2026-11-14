# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-over-a-map/.


def score_lines(scores: dict[str, int]) -> list[str]:
    """Return a line key=value for each entry in key order, then the total of the values."""
    # A dict gives its keys in insertion order, so sort the items.
    lines = [f"{key}={value}" for key, value in sorted(scores.items())]
    lines.append(f"total={sum(scores.values())}")
    return lines
