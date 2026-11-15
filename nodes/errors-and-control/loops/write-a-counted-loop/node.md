---
tag: W1ZXR4
kind: task
slug: write-a-counted-loop
name: Write a counted loop
aliases: [count from 1 to n, count to ten, counting loop, loop n times, repeat n times, for i in range, numeric for loop, Loops/For]
area: errors-and-control/loops
level: intro
summary: >-
  Count from 1 to n with a loop and collect each number, and know which range forms include the end and which stop one before it.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Loops-For/00-TASK.txt
  title: Loops/For, Rosetta Code (the RosettaCodeData mirror)
  locator: Task description
- url: https://docs.python.org/3/tutorial/controlflow.html
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.3 The range() Function
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Iteration statements, C# language reference
  locator: The for statement
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://doc.rust-lang.org/std/ops/struct.RangeInclusive.html
  title: RangeInclusive in std::ops, The Rust Standard Library
  locator: Description
- url: https://doc.rust-lang.org/std/ops/struct.Range.html
  title: Range in std::ops, The Rust Standard Library
  locator: Description
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: Bash Reference Manual
  locator: 3.5.1 Brace Expansion
---

## Task

Count from 1 to `n` with a loop. The function `count_to(n)` returns the list `[1, 2, ..., n]`. For example, `count_to(5)` is `[1, 2, 3, 4, 5]`, and `count_to(0)` is the empty list.

`n` is an integer from 0 to 1000. A counted loop sets a variable "to a monotonically increasing integer value for each execution of the block of code"[^1], so the solutions use a loop, not a library call that makes the list.

## Solutions

| Language | The loop |
|---|---|
| Python | `for i in range(1, n + 1)` |
| JavaScript, TypeScript | `for (let i = 1; i <= n; i++)` |
| Java, C#, C, C++ | `for (int i = 1; i <= n; i++)` |
| Go | `for i := 1; i <= n; i++` |
| Rust | `for i in 1..=n` |
| Bash | `for ((i = 1; i <= n; i++))` |

- **Python.** `range(1, n + 1)` gives 1 to `n`, because "The given end point is never part of the generated sequence"[^2].
- **JavaScript, TypeScript, Java, C#, C and C++.** The three-part `for` has a start, a test and a step. C# runs the body "while a specified Boolean expression evaluates to true"[^3], and the other languages have the same rule. The test `i <= n` includes `n`.
- **Go.** Go has only `for`. The solution uses the three-part form, because `for i := range n` starts at 0[^4].
- **Rust.** `1..=n` is "A range bounded inclusively below and above"[^5]. `1..n` would stop before `n`[^6].
- **C.** The function allocates `n` numbers, and the caller frees them.
- **Bash.** The arithmetic `for` counts from a variable. The function prints one number on each line.

See [for loop](/for-loop/) for the parts of the loop.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `zero`: the loop body does not run, and the result is empty. The test `i <= n` is false before the first pass.
- `one`: the start and the end are the same number, so the result has one item.
- `ten` and `three`: the last number is `n`. A loop with `i < n` gives one number too few. See [off-by-one errors](/off-by-one-errors/).

## Pitfalls

- **Python `range` stops before the end.** "The given end point is never part of the generated sequence"[^2], so `range(1, n)` misses `n`:

```python run id=python-range-end
n = 5
print(list(range(1, n)))
print(list(range(1, n + 1)))
```

```output for=python-range-end
[1, 2, 3, 4]
[1, 2, 3, 4, 5]
```

- **Rust `1..n` is half-open.** `Range` is "bounded inclusively below and exclusively above"[^6], and `1..=n` includes `n`[^5].
- **Go `range n` starts at 0.** "For an integer value n, where n is of integer type or an untyped integer constant, the iteration values 0 through n-1 are produced in increasing order"[^4]:

```go run id=go-range-int
package main

import "fmt"

func main() {
	n := 3
	var a, b []int
	for i := range n {
		a = append(a, i)
	}
	for i := 1; i <= n; i++ {
		b = append(b, i)
	}
	fmt.Println(a, b)
}
```

```output for=go-range-int
[0 1 2] [1 2 3]
```

- **Bash `{1..$n}` does not count.** "Brace expansion is performed before any other expansions"[^7], so `$n` is still text when the braces expand, and the loop runs once with the word `{1..5}`. Use the arithmetic `for` or `seq`:

```bash run id=bash-brace-variable
n=5
for i in {1..$n}; do echo "$i"; done
a=()
for ((i = 1; i <= n; i++)); do a+=("$i"); done
echo "${a[*]}"
```

```output for=bash-brace-variable
{1..5}
1 2 3 4 5
```

## Related tasks

- [[write-a-while-loop]]: a loop that stops on a condition, not a count.
- [[loop-with-a-step]]: count down, or count by 2 or more.
- [[loop-over-a-collection]]: visit each item of a list.
- [[fizzbuzz-task]]: a counted loop with a test in each pass.

## References
