# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-with-a-step/.

def count_by(start: int, stop: int, step: int) -> list[int]:
    """Return start, start + step, ... while the value has not passed stop."""
    # range stops before its end, so move the end one past stop.
    end = stop + 1 if step > 0 else stop - 1
    out = []
    for i in range(start, end, step):
        out.append(i)
    return out
