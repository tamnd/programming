---
tag: 3HE9AA
kind: task
slug: write-a-while-loop
name: Write a while loop
aliases: [while loop task, loop while a condition is true, halve until zero, repeat until, do-while task, Loops/While, Loops/Do-while]
area: errors-and-control/loops
level: intro
summary: >-
  Halve a number until it is 0 with a while loop, collect each value, and see why the loop needs integer division and why a do-while loop gives a different result for 0.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Loops-While/00-TASK.txt
  title: Loops/While, Rosetta Code (the RosettaCodeData mirror)
  locator: Task
- url: https://docs.python.org/3/tutorial/introduction.html
  title: An Informal Introduction to Python, The Python Tutorial
  locator: 3.1.1 Numbers
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Division
  title: Division (/), MDN Web Docs
  locator: Try it
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Iteration statements, C# language reference
  locator: The while statement; The do statement
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with single condition
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: Bash Reference Manual
  locator: 6.5 Shell Arithmetic
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.13 The do Statement
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Loops-Do-while/00-TASK.txt
  title: Loops/Do-while, Rosetta Code (the RosettaCodeData mirror)
  locator: Task description
---

## Task

Start with a number `n`, and halve it until it is 0. The function `halvings(n)` returns each value that the loop sees while the value is greater than 0. For example, `halvings(10)` is `[10, 5, 2, 1]`. Each step divides by 2 and drops the remainder.

`n` is an integer from 0 to 2147483647. The task follows Rosetta Code: "Loop while it is greater than zero"[^1]. The loop does not know the number of passes in advance, so it is a [while loop](/while-loop/), not a counted loop.

## Solutions

| Language | The loop | Integer halving |
|---|---|---|
| Python | `while n > 0:` | `n //= 2` |
| JavaScript, TypeScript | `while (n > 0)` | `n = Math.trunc(n / 2)` |
| Java, C#, C, C++ | `while (n > 0)` | `n /= 2` |
| Go | `for n > 0` | `n /= 2` |
| Rust | `while n > 0` | `n /= 2` |
| Bash | `while ((n > 0))` | `((n /= 2))` |

- **Python.** `/` "always returns a float. To do floor division and get an integer result you can use the // operator"[^2]. The solution uses `//`.
- **JavaScript and TypeScript.** A number can have a fraction, and `3 / 2` gives `1.5`[^3]. The solution drops the fraction with `Math.trunc`.
- **Java, C#, C, C++ and Rust.** The division of two integers gives an integer. A C# `while` loop "executes zero or more times"[^4], and the other languages have the same rule.
- **Go.** Go has no `while` keyword. A `for` statement with only a condition repeats the block while the condition is true[^5].
- **Bash.** Shell arithmetic works on integers only: "Evaluation is done in the largest fixed-width integers available"[^6]. The function prints one value on each line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero`: the test `n > 0` is false at the start, so the body does not run, and the result is empty.
- `one`: the loop runs once, and `1 / 2` is 0.
- `rosetta`: 1024 is a power of 2, so each value is exact, and the last value is 1.
- `ten` and `odd`: the remainder is dropped at each step, so 5 becomes 2.
- `limit`: 2147483647 gives 31 values. The values only get smaller, so no type overflows.

## Pitfalls

- **Python `/` makes a float, and the loop runs much longer.** With `n = n / 2`, the values become 0.5, 0.25 and so on. The loop stops only when the float is too small and becomes 0:

```python run id=python-float-halving
n = 1024
count = 0
while n > 0:
    count += 1
    last = n
    n = n / 2
print(count, last)

n = 1024
values = []
while n > 0:
    values.append(n)
    n = n // 2
print(len(values), values[-1])
```

```output for=python-float-halving
1085 5e-324
11 1
```

- **A do-while loop runs the body once before the test.** In Java, "A do statement is executed by first executing the Statement"[^7], and a C# `do` loop "executes one or more times"[^4]. The Rosetta Code do-while task needs this: "The loop must execute at least once"[^8]. For this task, it is wrong: with `n` equal to 0, a do-while loop gives `[0]`. See [do-while loop](/do-while-loop/).

```csharp run id=csharp-do-while-zero
int n = 0;
var a = new List<int>();
while (n > 0)
{
    a.Add(n);
    n /= 2;
}
n = 0;
var b = new List<int>();
do
{
    b.Add(n);
    n /= 2;
} while (n > 0);
Console.WriteLine($"while: [{string.Join(", ", a)}] do: [{string.Join(", ", b)}]");
```

```output for=csharp-do-while-zero
while: [] do: [0]
```

- **A body that does not change `n` never ends.** If the step is missing, or it changes a copy of `n`, the test stays true. See [infinite loops](/infinite-loops/).

## Related tasks

- [[write-a-counted-loop]]: a loop with a known number of passes.
- [[loop-with-a-step]]: a counted loop that goes down or by steps.
- [[fizzbuzz-task]]: a loop with a test in each pass.

## References
