# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-singleton/.

import threading


class IdGenerator:
    """Hands out consecutive ids. Get the one instance with IdGenerator.instance()."""

    _instance = None
    _lock = threading.Lock()
    constructions = 0  # how many times __init__ has run

    def __init__(self):
        IdGenerator.constructions += 1
        self._next = 1
        self._ids_lock = threading.Lock()

    @classmethod
    def instance(cls):
        if cls._instance is None:  # fast path, no lock after the first call
            with cls._lock:
                if cls._instance is None:  # another thread may have made it
                    cls._instance = cls()
        return cls._instance

    def next_id(self):
        with self._ids_lock:
            n = self._next
            self._next += 1
            return n


def singleton_report(calls):
    seen = []
    ids = []
    for _ in range(calls):
        g = IdGenerator.instance()
        if not any(g is s for s in seen):
            seen.append(g)
        ids.append(g.next_id())
    return [ids[-1] - ids[0], IdGenerator.constructions, len(seen)]
