---
tag: 3SQ6YR
kind: task
slug: destructure-a-value
name: Destructure a value
aliases: [unpack a tuple, sequence unpacking, destructuring assignment, structured bindings, record patterns, deconstruct a tuple, Manhattan distance]
area: errors-and-control/control-structures
level: core
summary: >-
  Take two points apart into the names x1, y1, x2 and y2 with one statement each, and return their Manhattan distance.
contract: contract.yaml
category: control-flow
languages: [python, javascript, typescript, java, c, cpp, csharp, go, rust, bash]
sources:
- url: https://docs.python.org/3.15/tutorial/datastructures.html#tuples-and-sequences
  title: Data Structures, The Python Tutorial
  locator: 5.3 Tuples and Sequences
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Destructuring
  title: Destructuring, MDN Web Docs
  locator: Description; Destructuring with more elements than the source; Swapping variables
- url: https://doc.rust-lang.org/book/ch19-01-all-the-places-for-patterns.html
  title: All the Places Patterns Can Be Used, The Rust Programming Language
  locator: let Statements
- url: https://doc.rust-lang.org/reference/patterns.html#slice-patterns
  title: Patterns, The Rust Reference
  locator: Slice patterns
- url: https://en.cppreference.com/w/cpp/language/structured_binding
  title: Structured binding declaration, cppreference.com
  locator: the first paragraph
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/operators/patterns#list-patterns
  title: Pattern matching, C# language reference
  locator: List patterns
- url: https://openjdk.org/jeps/440
  title: "JEP 440: Record Patterns"
  locator: Summary; Pattern matching and records; the header (Release 21)
- url: https://go.dev/ref/spec#Assignment_statements
  title: The Go Programming Language Specification
  locator: Assignment statements
- url: https://www.gnu.org/software/bash/manual/html_node/Bash-Builtins.html
  title: Bash Builtin Commands, Bash Reference Manual
  locator: read
- url: https://doc.rust-lang.org/book/ch19-02-refutability.html
  title: "Refutability: Whether a Pattern Might Fail to Match, The Rust Programming Language"
  locator: the first paragraphs
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/patterns/deconstruct
  title: Deconstructing tuples and other types, C# fundamentals
  locator: the first paragraph
- url: https://kotlinlang.org/docs/destructuring-declarations.html
  title: Destructuring declarations, Kotlin documentation
  locator: the first paragraphs
---

## Task

Take a compound value apart into names with one statement, in place of one index or one field access for each part. The function `manhattan(p, q)` gets two points. Each point is a list `[x, y]` of two integers. The function gives the names `x1` and `y1` to the parts of `p`, and `x2` and `y2` to the parts of `q`. Then it returns the Manhattan distance |x1 - x2| + |y1 - y2|.

For example, `manhattan([1, 2], [4, 6])` is 3 + 4 = 7.

The distance is not the important part. The important part is the statement that gives the names. Python calls it sequence unpacking, JavaScript and Kotlin call it destructuring, and C++ calls it a structured binding. Some languages have no such statement, and the solution reads each part by its index. [[pattern-match-on-a-value]] uses the same kind of pattern in a match, where the pattern can fail.

## Solutions

| Language | The statement for p | The mechanism |
|---|---|---|
| Python | `x1, y1 = p` | sequence unpacking[^1] |
| JavaScript | `const [x1, y1] = p;` | array destructuring[^2] |
| TypeScript | `const [x1, y1] = p;` | array destructuring of a tuple type `[number, number]` |
| Rust | `let [x1, y1] = p;` | a slice pattern in a `let` statement[^3][^4] |
| C++ | `const auto [x1, y1] = p;` | a structured binding[^5] |
| C# | `p is [var x1, var y1]` | a list pattern[^6] |
| Java | `case Segment(Point(int x1, int y1), Point(int x2, int y2)) ->` | a nested record pattern[^7] |
| Go | `x1, y1 := p[0], p[1]` | a tuple assignment[^8] |
| C | `int x1 = p[0], y1 = p[1];` | none; one index for each name |
| Bash | `read -r x1 y1` on the line of the array | `read` splits a line into words[^9] |

- **Python.** The tutorial names the statement `x, y, z = t`: "This is called, appropriately enough, sequence unpacking and works for any sequence on the right-hand side"[^1]. So `x1, y1 = p` works for a list, as it works for a tuple.
- **JavaScript and TypeScript.** "The destructuring syntax is a JavaScript syntax that makes it possible to unpack values from arrays, or properties from objects, into distinct variables"[^2]. In TypeScript, the type `[number, number]` tells the compiler that a point has two parts.
- **Rust.** A `let` statement takes a pattern, in the form `let PATTERN = EXPRESSION;`, and "Rust compares the expression against the pattern and assigns any names it finds"[^3]. The solution uses the array type `[i32; 2]`, so the pattern `[x1, y1]` always matches: "Slice patterns are irrefutable when matching an array as long as each element is irrefutable"[^4]. With a slice `&[i32]`, the pattern can fail. A slice pattern on a slice "is irrefutable only in the form with a single .. rest pattern"[^4]. A `let` statement accepts only a pattern that cannot fail[^10].
- **C++.** A structured binding "Binds the specified names to subobjects or elements of the initializer"[^5]. The solution uses `std::array<int, 2>`, so each point has exactly two elements, and the binding names both.
- **C#.** A list pattern tests a sequence, and "a list pattern matches when each nested pattern matches the corresponding element of an input sequence"[^6]. The pattern `[var x1, var y1]` also tests that the array has two elements, so the solution throws an exception for each other length. For tuples and records, C# also has a deconstruction, which assigns the parts of a value "to multiple variables in a single operation"[^11], such as `var (x1, y1) = point;`.
- **Java.** Java has no destructuring statement. The solution puts the two points in records, and a record pattern in a switch takes them apart. JEP 440 adds "record patterns to deconstruct record values", and "Record patterns and type patterns can be nested"[^7]. It is standard in Java 21[^7].
- **Go.** Go has no destructuring. "A tuple assignment assigns the individual elements of a multi-valued operation to a list of variables"[^8], so `x1, y1 := p[0], p[1]` gives the names in one statement. It still reads each part by its index.
- **C.** C has no destructuring and no tuple assignment, so the solution declares each name with one index.
- **Bash.** `read` puts the words of a line in names. It reads one line, and it will "split it into words as described above in Word Splitting, and assign the first word to the first name, the second word to the second name, and so on"[^9]. The solution joins the array into one line, and `read` splits it again.

### Objects and swaps

Destructuring also takes the properties of an object by name, and it can swap two variables. MDN states that "Without destructuring, swapping two values requires a temporary variable"[^2].

```javascript run id=js-object-and-swap
const point = { x: 3, y: -1 };
const { x, y } = point;

let a = 1;
let b = 3;
[a, b] = [b, a];

console.log(x, y, a, b);
```

```output for=js-object-and-swap
3 -1 3 1
```

Kotlin has the same statement for objects, the destructuring declaration: "A destructuring declaration creates multiple variables at once"[^12]. It calls the functions `component1()`, `component2()` and so on of the object, and a data class declares them[^12].

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `same-point`: the two points are equal, so the distance is 0.
- `negative`: all the coordinates are negative. The absolute value makes each difference positive.
- `large`: the coordinates are at the limits of the contract, -1000000 and 1000000. The distance is 4000000, which fits in a 32-bit integer.
- The contract states that each point has exactly 2 items. The pitfalls show what a point with a different length does in each language.

## Pitfalls

- **A missing element in JavaScript.** JavaScript does not check the length. If the array has fewer elements than the pattern has names, "only the first N variables are assigned values. The values of the remaining variables will be undefined"[^2]. A point with no y gives no error, but the distance becomes `NaN`:

```javascript wrong id=js-missing-element
function manhattan(p, q) {
  const [x1, y1] = p;
  const [x2, y2] = q;
  return Math.abs(x1 - x2) + Math.abs(y1 - y2);
}

console.log(manhattan([1, 2], [4, 6]));
console.log(manhattan([1], [4, 6]));
```

```output for=js-missing-element
7
NaN
```

Check the length before you destructure a value from outside the program, or use a default value such as `const [x1 = 0, y1 = 0] = p;`.

- **A length mismatch in other languages.** Python is strict: "Sequence unpacking requires that there are as many variables on the left side of the equals sign as there are elements in the sequence"[^1]. A list of 3 items makes `x1, y1 = p` raise a `ValueError`. Rust checks the length at compile time. "If the number of elements in the pattern doesn’t match the number of elements in the tuple, the overall type won’t match and we’ll get a compiler error"[^3]:

```rust fails expect=compile id=rust-tuple-too-long
fn main() {
    let (x, y) = (1, 2, 3);
    println!("{x} {y}");
}
```

- **Extra words in Bash.** `read` does not drop extra words: "If there are more words than names, the remaining words and their intervening delimiters are assigned to the last name"[^9]. With the line `3 4 5`, `read -r x y` sets `y` to `4 5`, and the arithmetic then fails.
- **A refutable pattern in a binding.** A `let` in Rust must take a pattern that always matches, because "Function parameters, let statements, and for loops can only accept irrefutable patterns"[^10]. `let [x1, y1] = p;` compiles for the array `[i32; 2]`, but not for a slice `&[i32]`, because a slice can have each length. For a slice, use a `match`, or the `let...else` statement, which accepts a pattern that can fail[^10].

## Related tasks

- [[pattern-match-on-a-value]]: the same patterns in a match, where a pattern can fail and the next case is tried.
- [[write-a-switch-or-match]]: a match on one value.

## References
