# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.


def levenshtein(a: str, b: str) -> int:
    """Return the minimum number of one-character insertions, deletions and
    substitutions that change a into b. Two rows of the table are kept."""
    prev = list(range(len(b) + 1))
    for i, ca in enumerate(a, 1):
        cur = [i]
        for j, cb in enumerate(b, 1):
            cur.append(min(prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + (ca != cb)))
        prev = cur
    return prev[-1]


if __name__ == "__main__":
    print(levenshtein("kitten", "sitting"))
