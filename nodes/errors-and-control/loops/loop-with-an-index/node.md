---
tag: KYHY8T
kind: task
slug: loop-with-an-index
name: Loop with an index
aliases: [loop with index and value, enumerate a list, find all positions, indexes of a value, for loop with index, entries, enumerate, iterate with index]
area: errors-and-control/loops
level: intro
summary: >-
  Loop over a list with the index and the item together, collect the index of each match, and avoid the loops that give only the first match or the wrong index.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3/tutorial/datastructures.html
  title: Data Structures, The Python Tutorial
  locator: 5.1 More on Lists; 5.6 Looping Techniques
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/entries
  title: Array.prototype.entries(), MDN Web Docs
  locator: Description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: Iterator in std::iter, The Rust Standard Library
  locator: enumerate
- url: https://en.cppreference.com/w/c/string/byte/strcmp
  title: strcmp, cppreference.com
  locator: Description
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: Bash Reference Manual
  locator: 3.5.3 Shell Parameter Expansion
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.21.3 Reference Equality Operators == and !=
---

## Task

Find each place where a value occurs in a list. The function `positions(items, target)` returns the 0-based index of each item that equals `target`, in increasing order. For example, `positions(["a", "b", "a", "c"], "a")` is `[0, 2]`. If no item matches, the result is empty.

`items` is a list of 0 to 1000 strings, and `target` is a string. Two strings are equal when they have the same characters, so the test is case-sensitive. The loop needs the index and the item in each pass.

## Solutions

| Language | The index and the item |
|---|---|
| Python | `for i, x in enumerate(items):` |
| JavaScript, TypeScript | `for (const [i, x] of items.entries())` |
| Go | `for i, x := range items` |
| Rust | `for (i, x) in items.iter().enumerate()` |
| Java, C#, C, C++ | `for (i = 0; i < n; i++)` and `items[i]` |
| Bash | `for i in "${!a[@]}"` and `${a[i]}` |

- **Python.** "the position index and corresponding value can be retrieved at the same time using the enumerate() function"[^1].
- **JavaScript and TypeScript.** `entries()` "returns a new array iterator object that contains the key/value pairs for each index in the array"[^2]. The loop takes each pair apart with `[i, x]`.
- **Go.** For a slice, `range` gives the index first and the item second[^3].
- **Rust.** `enumerate` "yields pairs (i, val) , where i is the current index of iteration and val is the value returned by the iterator"[^4]. The index is a `usize`.
- **Java, C#, C and C++.** A counted loop gives the index, and `items[i]` gives the item. Java compares with `equals`, and C with `strcmp`, which "Compares two null-terminated byte strings lexicographically"[^5] and returns 0 when they are equal.
- **Bash.** `${!name[@]}` "expands to the list of array indices (keys) assigned in name"[^6]. The function takes the target and then the items as arguments, and prints one index on each line.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: no items, so the loop body does not run.
- `none`: no item matches, so the result is empty.
- `all`: each item matches, so the result is each index.
- `last`: the match is the last item. A loop that stops at `n - 1` misses it. See [off-by-one errors](/off-by-one-errors/).
- `case`: `"A"` does not equal `"a"`.
- `empty-string`: the target is the empty string, and it matches only the empty items.

## Pitfalls

- **A search method finds only the first match.** Python `list.index` returns the "zero-based index of the first occurrence of value in the list" and "Raises a ValueError if there is no such item"[^1]. Use a loop to find each match:

```python run id=python-index-first
items = ["a", "b", "a", "c"]
print(items.index("a"))
print([i for i, x in enumerate(items) if x == "a"])
try:
    items.index("z")
except ValueError:
    print("ValueError")
```

```output for=python-index-first
0
[0, 2]
ValueError
```

- **Java `==` compares references, not the characters.** For two objects, "the result of == is true if the operand values are both null or both refer to the same object or array; otherwise, the result is false"[^7]. A string made at run time is a different object, so `==` misses it:

```java run id=java-string-equals
import java.util.ArrayList;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    String[] items = {"a", new String("a")};
    String target = "a";
    List<Integer> same = new ArrayList<>();
    List<Integer> equal = new ArrayList<>();
    for (int i = 0; i < items.length; i++) {
      if (items[i] == target) {
        same.add(i);
      }
      if (items[i].equals(target)) {
        equal.add(i);
      }
    }
    System.out.println("== " + same + " equals " + equal);
  }
}
```

```output for=java-string-equals
== [0] equals [0, 1]
```

- **Rust `enumerate` counts the items that it sees.** The index is "the current index of iteration"[^4], not the position in the list. If `enumerate` comes after `skip` or `filter`, the count starts again at 0. Put `enumerate` first:

```rust run id=rust-enumerate-order
fn main() {
    let items = ["x", "a", "b"];
    let late: Vec<(usize, &str)> = items.iter().copied().skip(1).enumerate().collect();
    let early: Vec<(usize, &str)> = items.iter().copied().enumerate().skip(1).collect();
    println!("{late:?}");
    println!("{early:?}");
}
```

```output for=rust-enumerate-order
[(0, "a"), (1, "b")]
[(1, "a"), (2, "b")]
```

- **The index of a Go string loop is a byte index.** For a string, `range` gives "the index of the first byte of successive UTF-8-encoded code points"[^3]. After a character outside ASCII, the index skips numbers. Loop over a slice of strings, as here, or convert the string to `[]rune` first.

See [for-each loop](/for-each-loop/) for the loop without an index.

## Related tasks

- [[loop-over-a-collection]]: visit each item without the index.
- [[write-a-counted-loop]]: a loop over the numbers alone.
- [[loop-over-nested-loops]]: two indexes, one for the row and one for the column.

## References
