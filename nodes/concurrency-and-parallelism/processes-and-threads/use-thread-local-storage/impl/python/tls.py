# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-thread-local-storage/.

"""Count on a thread-local counter in one thread for each count, with no lock and no shared counter."""

import threading

# One object for all threads. Its attributes have a separate value in each thread.
_local = threading.local()


def bump() -> int:
    # A new thread sees no attribute yet, so its counter starts at 0.
    _local.counter = getattr(_local, "counter", 0) + 1
    return _local.counter


def current() -> int:
    return getattr(_local, "counter", 0)


def count_per_thread(counts: list[int]) -> list[int]:
    results = [0] * len(counts)

    def work(i: int) -> None:
        for _ in range(counts[i]):
            bump()
        # Each thread writes only its own slot of the list.
        results[i] = current()

    threads = [threading.Thread(target=work, args=(i,)) for i in range(len(counts))]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    return results
