# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/start-a-thread/.

"""Start one thread per name; each thread builds the greeting for its name."""

import threading


def _greet(name: str, results: list[str], index: int) -> None:
    # Runs on the new thread. It writes only its own slot of the list.
    results[index] = f"hello from {name}"


def thread_greetings(names: list[str]) -> list[str]:
    results = [""] * len(names)
    threads = [
        threading.Thread(target=_greet, args=(name, results, i), name=f"greeter-{i}")
        for i, name in enumerate(names)
    ]
    for thread in threads:
        thread.start()
    # Start all threads first, then wait for each one, so that they can run at the same time.
    for thread in threads:
        thread.join()
    return results
