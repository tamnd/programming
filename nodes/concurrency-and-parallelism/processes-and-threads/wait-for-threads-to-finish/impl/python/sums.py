# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

"""Sum each chunk on its own worker thread, wait for all of them and collect the sums."""

from concurrent.futures import ThreadPoolExecutor


def parallel_sums(chunks: list[list[int]]) -> list[int]:
    if not chunks:
        return []
    # One worker thread for each chunk. Leaving the with block waits for all of them.
    with ThreadPoolExecutor(max_workers=len(chunks)) as executor:
        futures = [executor.submit(sum, chunk) for chunk in chunks]
    # Every future is done here; result() returns the sum, or raises the exception of the call.
    return [future.result() for future in futures]
