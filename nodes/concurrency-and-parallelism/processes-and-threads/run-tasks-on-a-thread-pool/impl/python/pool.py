# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

"""Submit one job for each number to a fixed pool of worker threads and collect the results in order."""

from concurrent.futures import ThreadPoolExecutor


def collatz_steps(n: int) -> int:
    steps = 0
    while n != 1:
        n = n // 2 if n % 2 == 0 else 3 * n + 1
        steps += 1
    return steps


def steps_on_pool(workers: int, numbers: list[int]) -> list[int]:
    # The pool starts at most `workers` threads and gives each queued job to a free thread.
    # Leaving the with block waits for all jobs and stops the threads.
    with ThreadPoolExecutor(max_workers=workers) as pool:
        futures = [pool.submit(collatz_steps, n) for n in numbers]
    # Every future is done here; result() returns the value, or raises the exception of the job.
    return [future.result() for future in futures]
