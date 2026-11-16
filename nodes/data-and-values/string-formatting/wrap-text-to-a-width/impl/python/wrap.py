# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/wrap-text-to-a-width/.


def wrap(text: str, width: int) -> list[str]:
    """Return the lines of text, each at most width characters long, with
    the greedy method. A word longer than width is a line of its own."""
    lines: list[str] = []
    line = ""
    for word in text.split():
        if not line:
            line = word
        elif len(line) + 1 + len(word) <= width:
            line += " " + word
        else:
            lines.append(line)
            line = word
    if line:
        lines.append(line)
    return lines


if __name__ == "__main__":
    print("\n".join(wrap("The quick brown fox jumps over the lazy dog", 10)))
