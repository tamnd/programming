# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-conditional/.

def grade(score: int) -> str:
    """Return the letter grade for a score from 0 to 100."""
    # The tests go from the highest bound down, so each test needs only one bound.
    if score >= 90:
        return "A"
    elif score >= 80:
        return "B"
    elif score >= 70:
        return "C"
    elif score >= 60:
        return "D"
    else:
        return "F"
