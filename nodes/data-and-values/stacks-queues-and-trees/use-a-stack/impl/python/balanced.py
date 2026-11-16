# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-stack/.

PAIRS = {")": "(", "]": "[", "}": "{"}


def balanced(text: str) -> bool:
    """Returns True if the brackets of text match and nest correctly."""
    stack: list[str] = []  # a list is the stack: append pushes, pop pops
    for c in text:
        if c in "([{":
            stack.append(c)
        elif c in PAIRS:
            if not stack or stack[-1] != PAIRS[c]:  # stack[-1] is the top
                return False
            stack.pop()
    return not stack  # an opening bracket that is left has no partner
