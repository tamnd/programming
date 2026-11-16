# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-queue/.

from collections import deque


def finish_order(work: list[int], slice: int) -> list[int]:
    """Returns the job numbers in the order in which round-robin finishes them."""
    left = list(work)  # a copy, so the list of the caller does not change
    queue = deque(range(len(work)))  # not a list: list.pop(0) is slow
    done = []
    while queue:
        job = queue.popleft()  # dequeue from the front
        left[job] -= min(slice, left[job])
        if left[job] == 0:
            done.append(job)
        else:
            queue.append(job)  # enqueue at the back
    return done
