---
tag: 36PTCF
kind: task
slug: loop-over-nested-loops
name: Loop over nested loops
aliases: [nested loops, loop over a grid, search a 2D array, break out of nested loops, labeled break, break outer loop, row and column loop, Loops/Nested]
area: errors-and-control/loops
level: intro
summary: >-
  Search a grid with a loop inside a loop, stop both loops at the first match, and know which break leaves only the inner loop.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Loops-Nested/00-TASK.txt
  title: Loops/Nested, Rosetta Code (the RosettaCodeData mirror)
  locator: Task
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.15 The break Statement
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/label
  title: Labeled statement, MDN Web Docs
  locator: Description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Break statements
- url: https://doc.rust-lang.org/book/ch03-05-control-flow.html
  title: Control Flow, The Rust Programming Language
  locator: Disambiguating with Loop Labels
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/jump-statements
  title: Jump statements, C# language reference
  locator: The break statement
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: Bash Reference Manual
  locator: 4.1 Bourne Shell Builtins
- url: https://docs.python.org/3/reference/simple_stmts.html
  title: Simple statements, The Python Language Reference
  locator: 7.9 The break statement
- url: https://docs.python.org/3/tutorial/controlflow.html
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.5 else Clauses on Loops
---

## Task

Search a grid of numbers for a value. The function `find_in_grid(grid, target)` loops over the rows, and for each row over the columns, and returns the row and the column of the first cell that equals `target`. If no cell matches, it returns `(-1, -1)`. For example, in `[[1, 2, 3], [4, 5, 6]]` the value 5 is at `(1, 1)`.

`grid` has 0 to 100 rows, and each row has 0 to 100 integers. The rows can have different lengths. "First" means in row-major order: each cell of row 0 comes before each cell of row 1. The Rosetta Code task searches a two-dimensional array in the same way, and asks how to break out of the nested loops[^1].

## Solutions

| Language | Stop both loops |
|---|---|
| Java, JavaScript, TypeScript | `outer:` before the outer loop, `break outer;` |
| Go | `outer:` before the outer loop, `break outer` |
| Rust | `'outer:` before the outer loop, `break 'outer;` |
| Python, C#, C, C++ | `return` from inside the inner loop |
| Bash | `break 2` |

- **Java.** A break with a label "attempts to transfer control to the enclosing labeled statement (§14.7) that has the same Identifier as its label"[^2]. The solution labels the outer loop and breaks to it.
- **JavaScript and TypeScript.** "You can jump to this label using a break or continue statement nested within the labeled statement"[^3].
- **Go.** `break` can take the label of an enclosing loop, and "that is the one whose execution terminates"[^4].
- **Rust.** "You can optionally specify a loop label on a loop that you can then use with break or continue to specify that those keywords apply to the labeled loop instead of the innermost loop"[^5].
- **C#.** "In nested loops, the break statement terminates only the innermost loop that contains it"[^6]. The search is a function of its own, so `return` leaves both loops. Python, C and C++ use `return` in the same way.
- **Bash.** "If n is supplied, break exits the nth enclosing loop"[^7], so `break 2` leaves both loops. The function takes the target and then each row as one string of numbers, and prints the row and the column.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `first-cell` and `last-cell`: the match is in the first or the last cell, so the loops must include both ends.
- `row-major`: the value is at `(0, 2)` and at `(1, 0)`. The row loop is the outer loop, so the result is `(0, 2)`. A loop with the column outside would give `(1, 0)`.
- `missing`: no cell matches, so both loops run to the end.
- `empty-grid` and `empty-rows`: the inner loop runs 0 times for an empty row. The search goes on to the next row.
- `ragged`: the rows have different lengths. The inner loop uses the length of its own row, not of row 0.
- `negative`: the grid and the target can be negative.

## Pitfalls

- **A plain `break` leaves only the inner loop.** In Python, `break` "terminates the nearest enclosing loop"[^8]. The outer loop goes on, so the code below finds 3 in each row, not only once. The `else` of a `for` loop runs "if no break occurred"[^9], and a `break` after it stops the outer loop too. A function with `return` is easier to read:

```python run id=python-break-inner
grid = [[1, 2], [3, 4], [3, 5]]
found = []
for r, row in enumerate(grid):
    for c, x in enumerate(row):
        if x == 3:
            found.append((r, c))
            break
print(found)

for r, row in enumerate(grid):
    for c, x in enumerate(row):
        if x == 3:
            break
    else:
        continue
    break
print(r, c)
```

```output for=python-break-inner
[(1, 0), (2, 0)]
1 0
```

- **In Go, `break` inside a `switch` leaves the `switch`.** An unlabeled `break` ends the innermost `for`, `switch` or `select` statement[^4]. In the first loop below, the `break` ends the `switch`, so the loops visit all 4 cells. The labeled `break` stops after 2 cells:

```go run id=go-break-switch
package main

import "fmt"

func main() {
	grid := [][]int{{1, 2}, {3, 4}}
	n := 0
	for _, row := range grid {
		for _, x := range row {
			n++
			switch {
			case x == 2:
				break
			}
		}
	}
	m := 0
outer:
	for _, row := range grid {
		for _, x := range row {
			m++
			switch {
			case x == 2:
				break outer
			}
		}
	}
	fmt.Println(n, m)
}
```

```output for=go-break-switch
4 2
```

- **Bash `break` also leaves one loop.** Without `n`, the outer loop goes on to the next row:

```bash run id=bash-break-n
for row in "1 2" "3 4"; do
  for x in $row; do
    if ((x == 2)); then break; fi
    echo "a $x"
  done
done
for row in "1 2" "3 4"; do
  for x in $row; do
    if ((x == 2)); then break 2; fi
    echo "b $x"
  done
done
```

```output for=bash-break-n
a 1
a 3
a 4
b 1
```

## Related tasks

- [[loop-with-an-index]]: one index for one list.
- [[loop-over-a-collection]]: one loop over one list.
- [[loop-over-several-collections-at-once]]: one loop that walks several lists side by side.

## References
