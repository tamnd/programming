# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/align-columns-of-text/.
"""Aligns the columns of a text in which a $ separates the fields."""

ALIGNS = ("left", "right", "center")


def align_columns(text: str, align: str) -> str:
    """Returns text with its fields in columns.

    A line ends at LF. Each $ in a line separates two fields, and a $ at the
    end of a line does not start a new field. An empty line has no fields. The
    width of a column is the largest number of code points of a field in it.
    Each field is padded with spaces to the width of its column, on the right
    (left), on the left (right), or on the two sides (center), with the odd
    space on the right. One space separates two columns. The padding on the
    right of the last field of a line is not written. Each line of the result
    ends with LF. Raises ValueError if align is not left, right or center.
    """
    if align not in ALIGNS:
        raise ValueError(f"bad align: {align!r}")
    lines = text.split("\n")
    if lines[-1] == "":
        # A final LF ends the last line. It does not start a new line.
        lines.pop()
    rows = [split_fields(line) for line in lines]
    widths: list[int] = []
    for row in rows:
        for j, field in enumerate(row):
            if j == len(widths):
                widths.append(0)
            widths[j] = max(widths[j], len(field))
    out = []
    for row in rows:
        parts = []
        for j, field in enumerate(row):
            pad = widths[j] - len(field)
            left = {"left": 0, "right": pad, "center": pad // 2}[align]
            right = pad - left if j < len(row) - 1 else 0
            parts.append(" " * left + field + " " * right)
        out.append(" ".join(parts) + "\n")
    return "".join(out)


def split_fields(line: str) -> list[str]:
    """Returns the fields of a line: none for an empty line."""
    if line == "":
        return []
    fields = line.split("$")
    if line.endswith("$"):
        fields.pop()
    return fields
