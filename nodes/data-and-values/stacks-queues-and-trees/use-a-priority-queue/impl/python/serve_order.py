# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-priority-queue/.

import heapq


def serve_order(priorities: list[int]) -> list[int]:
    """Returns the request numbers in serve order: the largest priority
    first, and for equal priorities, the smaller request number first."""
    heap: list[tuple[int, int]] = []
    for i, p in enumerate(priorities):
        # heapq is a min-heap, so -p puts the largest priority on top. The
        # request number i breaks ties and keeps the arrival order.
        heapq.heappush(heap, (-p, i))
    return [heapq.heappop(heap)[1] for _ in range(len(heap))]
