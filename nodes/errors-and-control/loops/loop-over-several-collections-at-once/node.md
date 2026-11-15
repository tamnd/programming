---
tag: MW1A52
kind: task
slug: loop-over-several-collections-at-once
name: Loop over several collections at once
aliases: [loop over multiple arrays, iterate over three lists, parallel iteration, lockstep loop, zip three lists, loop over arrays simultaneously, Loop over multiple arrays simultaneously]
area: errors-and-control/loops
level: intro
summary: >-
  Walk three lists side by side in one loop, join the items at each index, and stop at the end of the shortest list instead of reading past the end of one.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Loop-over-multiple-arrays-simultaneously/00-TASK.txt
  title: Loop over multiple arrays simultaneously, Rosetta Code (the RosettaCodeData mirror)
  locator: Task
- url: https://docs.python.org/3/library/functions.html
  title: Built-in Functions, The Python Standard Library
  locator: zip()
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.zip
  title: Enumerable.Zip Method, .NET API reference
  locator: Zip<TFirst,TSecond,TThird>
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: Iterator in std::iter, The Rust Standard Library
  locator: zip
- url: https://en.cppreference.com/w/cpp/ranges/zip_view
  title: std::ranges::zip_view, std::views::zip, cppreference.com
  locator: Description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: Min and max
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: Bash Reference Manual
  locator: 3.4 Shell Parameters
---

## Task

Loop over three lists at the same time, and join the items that have the same index. The function `zip_join(lower, upper, digits)` returns, for each index `i`, the string `lower[i] + upper[i] + digits[i]`. This is the example of the Rosetta Code task: the lists `(a, b, c)`, `(A, B, C)` and `(1, 2, 3)` give `aA1`, `bB2` and `cC3`[^1].

`lower` and `upper` are lists of strings, and `digits` is a list of 32-bit signed integers. Each list has 0 to 1000 items. The lists can have different lengths, and the task asks what happens then[^1]. Here the loop stops at the end of the shortest list, so the result has as many items as the shortest list.

## Solutions

| Language | One loop over three lists |
|---|---|
| Python | `for a, b, d in zip(lower, upper, digits):` |
| C# | `foreach (var (a, b, d) in lower.Zip(upper, digits))` |
| Rust | `for ((a, b), d) in lower.iter().zip(upper).zip(digits)` |
| C++ | `for (auto [a, b, d] : std::views::zip(lower, upper, digits))` |
| Go | `for i := range min(len(lower), len(upper), len(digits))` |
| JavaScript, TypeScript, Java, C | an index from 0 to the smallest length |
| Bash | three array names, an index to the smallest length |

- **Python.** `zip` takes any number of lists. "By default, zip() stops when the shortest iterable is exhausted"[^2].
- **C#.** One overload of `Zip` takes three sequences and "Produces a sequence of tuples with elements from the three specified sequences"[^3]. "If the sequences do not have the same number of elements, the method merges sequences until it reaches the end of one of them"[^3].
- **Rust.** `zip` joins two iterators, "returning a tuple where the first element comes from the first iterator, and the second element comes from the second iterator"[^4]. For three lists, the solution zips twice, so each item is a pair inside a pair.
- **C++.** `zip_view` makes a view "whose i th element is a tuple-like value consisting of the i th elements of all views", and "The size of produced view is the minimum of sizes of all adapted views"[^5].
- **Go.** The built-in `min` takes "a fixed number of arguments of ordered types"[^6], so one call gives the smallest of the three lengths.
- **JavaScript, TypeScript, Java and C.** The solution calculates the smallest length first, and then uses an index that is in all three lists.
- **Bash.** A function cannot take three arrays as arguments, so it takes the names of the arrays. A nameref is "a reference to another variable"[^7], and the function reads the arrays through three namerefs. It prints one string on each line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `shorter-first` and `shorter-last`: the shortest list is the first or the last one. The result has one item in both cases.
- `one-empty` and `all-empty`: when one list is empty, the result is empty.
- `negative`: a negative number keeps its sign, so the string is `aA-5`.
- `empty-strings`: an empty string adds nothing, so `"" + "A" + 1` is `A1`.

## Pitfalls

- **An index loop with the length of one list reads past the end of a shorter list.** In JavaScript, the missing item is `undefined`, and `+` turns it into text. Use the smallest length:

```javascript run id=js-index-past-end
const lower = ["a", "b", "c"];
const upper = ["A", "B"];
const digits = [1, 2, 3];
const out = [];
for (let i = 0; i < lower.length; i++) {
  out.push(lower[i] + upper[i] + digits[i]);
}
console.log(out.join(" "));
const n = Math.min(lower.length, upper.length, digits.length);
console.log(n);
```

```output for=js-index-past-end
aA1 bB2 cundefined3
2
```

- **Python `zip` drops the extra items without an error.** If the lists must have the same length, use `strict=True`: "If strict is True and one of the iterables is exhausted before the others, a ValueError is raised"[^2].

```python run id=python-zip-three
lower = ["a", "b", "c"]
upper = ["A", "B", "C"]
digits = [1, 2]
print([f"{a}{b}{d}" for a, b, d in zip(lower, upper, digits)])
try:
    list(zip(lower, upper, digits, strict=True))
except ValueError:
    print("ValueError")
```

```output for=python-zip-three
['aA1', 'bB2']
ValueError
```

- **Rust `zip` of three lists gives nested pairs.** Each `zip` makes a pair[^4], so the second `zip` makes a pair whose first part is a pair. The pattern must be `((a, b), d)`, not `(a, b, d)`:

```rust run id=rust-zip-nested
fn main() {
    let lower = ["a", "b"];
    let upper = ["A", "B"];
    let digits = [1, 2, 3];
    let items: Vec<_> = lower.iter().zip(upper.iter()).zip(digits.iter()).collect();
    println!("{items:?}");
}
```

```output for=rust-zip-nested
[(("a", "A"), 1), (("b", "B"), 2)]
```

## Related tasks

- [[zip-two-lists]]: pair the items of two lists.
- [[loop-with-an-index]]: one list, with the index.
- [[loop-over-a-collection]]: one list, without the index.
- [[loop-over-nested-loops]]: a loop inside a loop, not side by side.

## References
