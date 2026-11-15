# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/fizzbuzz-task/.

def fizzbuzz(n: int) -> list[str]:
    """Return FizzBuzz, Fizz, Buzz or the number for each number from 1 to n."""
    out = []
    for i in range(1, n + 1):
        # Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
        if i % 15 == 0:
            out.append("FizzBuzz")
        elif i % 3 == 0:
            out.append("Fizz")
        elif i % 5 == 0:
            out.append("Buzz")
        else:
            out.append(str(i))
    return out
