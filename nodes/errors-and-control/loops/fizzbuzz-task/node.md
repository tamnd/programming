---
tag: 060T36
kind: task
slug: fizzbuzz-task
name: FizzBuzz
aliases: [fizzbuzz, fizz buzz, Fizz Buzz, FizzBuzz task, fizzbuzz interview question, count with fizz and buzz]
area: errors-and-control/loops
level: intro
summary: >-
  Count from 1 to n, and say Fizz for multiples of 3, Buzz for multiples of 5 and FizzBuzz for both, with the test for both before the other two.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/FizzBuzz/00-TASK.txt
  title: FizzBuzz, Rosetta Code (the RosettaCodeData mirror)
  locator: Task
- url: https://en.wikipedia.org/wiki/Fizz_buzz
  title: Fizz buzz, Wikipedia
  locator: Lead; Play; Programming
- url: https://docs.python.org/3/tutorial/introduction.html
  title: An Informal Introduction to Python, The Python Tutorial
  locator: 3.1.1 Numbers
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: 8.1 The if statement
---

## Task

Count from 1 to `n`, and replace some numbers with words. The function `fizzbuzz(n)` returns one string for each number from 1 to `n`:

- `"FizzBuzz"` for a multiple of both 3 and 5,
- `"Fizz"` for a multiple of 3,
- `"Buzz"` for a multiple of 5,
- the number in decimal for any other number.

For example, `fizzbuzz(5)` is `["1", "2", "Fizz", "4", "Buzz"]`. `n` is an integer from 0 to 10000.

The Rosetta Code task prints the integers from 1 to 100 with the same rules[^1]. Fizz buzz is "a group word game for children to teach them about division", and it "has been used as an interview screening device for computer programmers"[^2]. The program needs "little more than a loop and conditional statements in any popular language"[^2]. Here the function returns the strings, so the tests can check them.

## Solutions

| Language | The loop | The test for both |
|---|---|---|
| Python | `for i in range(1, n + 1):` | `i % 15 == 0` |
| JavaScript, TypeScript | `for (let i = 1; i <= n; i++)` | `i % 15 === 0` |
| Java, C#, C, C++ | `for (int i = 1; i <= n; i++)` | `i % 15 == 0` |
| Go | `for i := 1; i <= n; i++` | `i%15 == 0` |
| Rust | `for i in 1..=n` | `i % 15 == 0` |
| Bash | `for ((i = 1; i <= n; i++))` | `((i % 15 == 0))` |

- **The remainder.** `%` gives the remainder of a division. In Python, "the % operator returns the remainder of the division"[^3], and the other languages have the same operator for these positive numbers. A number is a multiple of 3 when `i % 3` is 0.
- **The order of the tests.** A multiple of both 3 and 5 is a multiple of 15. Each solution tests 15 first, then 3, then 5, with one `if` and `else if` chain.
- **C.** Each string is a new allocation, and the caller frees each one and the array.
- **Bash.** The function prints one string on each line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero`: the loop body does not run, and the result is empty.
- `one`: only the number 1.
- `three` and `five`: the first `Fizz` and the first `Buzz` are the last item.
- `fifteen`: the first `FizzBuzz` is the last item. The loop must include `n`. See [off-by-one errors](/off-by-one-errors/).
- `thirty`: two `FizzBuzz` items.

## Pitfalls

- **The test for 3 before the test for 15 hides `FizzBuzz`.** An `if` chain "selects exactly one of the suites by evaluating the expressions one by one until one is found to be true"[^4]. If `i % 3 == 0` comes first, it is true for 15, and the chain never reaches the test for 15:

```python run id=python-fizzbuzz-order
def wrong(i):
    if i % 3 == 0:
        return "Fizz"
    elif i % 5 == 0:
        return "Buzz"
    elif i % 15 == 0:
        return "FizzBuzz"
    return str(i)


def right(i):
    if i % 15 == 0:
        return "FizzBuzz"
    elif i % 3 == 0:
        return "Fizz"
    elif i % 5 == 0:
        return "Buzz"
    return str(i)


print([wrong(i) for i in range(13, 16)])
print([right(i) for i in range(13, 16)])
```

```output for=python-fizzbuzz-order
['13', '14', 'Fizz']
['13', '14', 'FizzBuzz']
```

- **The loop starts at 1 and includes `n`.** A loop from 0 adds an item for 0, which is a multiple of 15, and a loop that stops before `n` misses the last item. See [write a counted loop](/write-a-counted-loop/).

## Related tasks

- [[write-a-counted-loop]]: the loop from 1 to `n`.
- [[write-a-while-loop]]: a loop that stops on a condition.
- [[loop-with-a-step]]: count by 3 or by 5.

## References
