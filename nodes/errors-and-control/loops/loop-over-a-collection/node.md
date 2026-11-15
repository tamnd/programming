---
tag: RG9JMG
kind: task
slug: loop-over-a-collection
name: Loop over a collection
aliases: [for each item, iterate over a list, iterate over an array, foreach task, for-of loop, enhanced for loop, range over a slice, sum a list with a loop, Loops/Foreach]
area: errors-and-control/loops
level: intro
summary: >-
  Visit each item of a list with a for-each loop and add the items, and know which loops give the items, which give the keys, and when the sum needs a wider type.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://github.com/acmeism/RosettaCodeData/blob/main/Task/Loops-Foreach/00-TASK.txt
  title: Loops/Foreach, Rosetta Code (the RosettaCodeData mirror)
  locator: Task
- url: https://docs.python.org/3/tutorial/controlflow.html
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.2 for Statements
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...of
  title: for...of, MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.14.2 The enhanced for statement
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Iteration statements, C# language reference
  locator: The foreach statement
- url: https://en.cppreference.com/w/cpp/language/range-for.html
  title: Range-based for loop, cppreference.com
  locator: Description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://doc.rust-lang.org/book/ch03-05-control-flow.html
  title: Control Flow, The Rust Programming Language
  locator: Looping Through a Collection with for
- url: https://www.gnu.org/software/bash/manual/bash.html
  title: Bash Reference Manual
  locator: 3.2.5.1 Looping Constructs
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...in
  title: for...in, MDN Web Docs
  locator: Description
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-15.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 15
  locator: 15.18.2 Additive Operators (+ and -) for Numeric Types
- url: https://docs.python.org/3/library/stdtypes.html
  title: Built-in Types, The Python Standard Library
  locator: Mapping Types, dict
---

## Task

Visit each item of a list in order, and add the items. The function `total(items)` returns the sum of the items, and 0 for the empty list. For example, `total([1, 2, 3])` is 6.

`items` has from 0 to 1000 items, and each item is a 32-bit signed integer. The sum can be outside the 32-bit range, so the result is a 64-bit integer. Rosetta Code asks for a for-each loop when the language has one[^1], so the solutions use one in each language except C.

## Solutions

| Language | The loop |
|---|---|
| Python | `for x in items:` |
| JavaScript, TypeScript | `for (const x of items)` |
| Java | `for (int x : items)` |
| C# | `foreach (int x in items)` |
| C++ | `for (int x : items)` |
| Go | `for _, x := range items` |
| Rust | `for &x in items` |
| Bash | `for x in "$@"` |
| C | `for (size_t i = 0; i < n; i++)` |

- **Python.** The `for` statement "iterates over the items of any sequence (a list or a string), in the order that they appear in the sequence"[^2].
- **JavaScript and TypeScript.** `for...of` "executes a loop that operates on a sequence of values sourced from an iterable object"[^3]. A sum of 1000 32-bit numbers is far below 2^53, so a `number` holds it exactly.
- **Java.** The enhanced `for` takes "an array type (§10.1) or a subtype of the raw type Iterable"[^4]. The sum is a `long`.
- **C#.** `foreach` runs the body "for each element in an instance of the type that implements the System.Collections.IEnumerable or System.Collections.Generic.IEnumerable<T> interface"[^5]. The sum is a `long`.
- **C++.** The range-based `for` "Executes a for loop over a range"[^6]. The sum is a `long long`.
- **Go.** A `for` with a `range` clause gives the index and the value of each item[^7]. The solution drops the index with `_`. The items are `int32` and the sum is `int64`.
- **Rust.** "you can use a for loop and execute some code for each item in a collection"[^8]. `for &x in items` copies each `i32` out of the slice. The sum is `i64`.
- **Bash.** `for name in words` runs the commands "once for each word in the resultant list"[^9]. The function takes the items as arguments, so the loop reads `"$@"`.
- **C.** The function gets a pointer and a length, so it uses an index loop.

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `empty`: the loop body does not run, and the sum stays 0.
- `single`: one item, so the sum is the item.
- `negative`: the items can be negative, and the sum can be negative.
- `limit` and `limit-low`: the sum is outside the 32-bit range. A 32-bit sum wraps around and gives a wrong answer.

## Pitfalls

- **JavaScript `for...in` gives the keys, not the items.** It "iterates over all enumerable string properties of an object"[^10], and "Array indexes are just enumerable properties with integer names"[^10]. So `x` is the string `"0"`, then `"1"`, and `+` joins strings:

```javascript run id=js-for-in-keys
const items = [1, 2, 3];
let a = 0;
for (const x in items) {
  a += x;
}
let b = 0;
for (const x of items) {
  b += x;
}
console.log(a, b);
```

```output for=js-for-in-keys
0012 6
```

- **A 32-bit sum wraps around.** In Java, "If an integer addition overflows, then the result is the low-order bits of the mathematical sum"[^11]. Use a 64-bit sum:

```java run id=java-int-sum
public class Main {
  public static void main(String[] args) {
    int[] items = {2147483647, 2147483647, 2};
    int bad = 0;
    long good = 0;
    for (int x : items) {
      bad += x;
      good += x;
    }
    System.out.println(bad + " " + good);
  }
}
```

```output for=java-int-sum
0 4294967296
```

- **The order of a map loop depends on the language.** In Go, "The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next"[^7]. In Python, "Dictionaries preserve insertion order"[^12]. If the order matters, loop over a sorted list of the keys.
- **Do not add or remove items in the loop.** A for-each loop can skip items or fail when the collection changes under it. See [iterator invalidation](/iterator-invalidation/).

See [for-each loop](/for-each-loop/) for the loop itself.

## Related tasks

- [[write-a-counted-loop]]: a loop over numbers, not items.
- [[loop-with-an-index]]: the index and the item together.
- [[loop-over-several-collections-at-once]]: one loop over two or more lists.
- [[reverse-a-list]]: walk a list from the end.

## References
