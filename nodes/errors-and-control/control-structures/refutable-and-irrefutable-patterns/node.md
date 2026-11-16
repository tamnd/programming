---
tag: BM4PBY
kind: concept
slug: refutable-and-irrefutable-patterns
name: Refutable and irrefutable patterns
aliases: [refutability, refutable pattern, irrefutable pattern, unconditional pattern, if let, while let, let else, let-else]
area: errors-and-control/control-structures
level: advanced
summary: >-
  A pattern is refutable when it can fail to match a value, and irrefutable when it always matches. A binding such as let needs an irrefutable pattern; a test such as if let takes a refutable one.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2022
  people: []
  note: Rust 1.65, released on 3 November 2022, added let-else, a let statement with a refutable pattern and an else block that must diverge.
see_also: [pattern-matching, destructure-a-value, exhaustive-matching, match-guards, pattern-match-on-a-value]
sources:
- url: https://doc.rust-lang.org/book/ch19-02-refutability.html
  title: "Refutability: Whether a Pattern Might Fail to Match, The Rust Programming Language"
  locator: the whole section
- url: https://doc.rust-lang.org/reference/patterns.html#refutability
  title: Patterns, The Rust Reference
  locator: Refutability; the notes on refutability in each kind of pattern
- url: https://doc.rust-lang.org/error_codes/E0005.html
  title: Error code E0005, Rust error codes index
  locator: the whole page
- url: https://doc.rust-lang.org/reference/statements.html#let-statements
  title: Statements, The Rust Reference
  locator: let statements
- url: https://docs.python.org/3.15/reference/compound_stmts.html#irrefutable-case-blocks
  title: Compound statements, The Python Language Reference
  locator: 8.6.3 Irrefutable Case Blocks
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Destructuring
  title: Destructuring, MDN Web Docs
  locator: Destructuring with more elements than the source; Destructuring primitive values
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.30.3
  title: The Java Language Specification, Java SE 25 Edition
  locator: 14.30.3 Properties of Patterns
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/patterns/deconstruct
  title: Deconstructing tuples and other types, C# fundamentals
  locator: the first paragraph
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch3.html#x8-580003.17
  title: Haskell 2010 Language Report, Chapter 3 Expressions
  locator: 3.17.2 Informal Semantics of Pattern Matching
- url: https://docs.python.org/3.15/tutorial/datastructures.html#tuples-and-sequences
  title: Data Structures, The Python Tutorial
  locator: 5.3 Tuples and Sequences
- url: https://doc.rust-lang.org/book/ch06-03-if-let.html
  title: Concise Control Flow with if let and let...else, The Rust Programming Language
  locator: the first part
- url: https://doc.rust-lang.org/book/ch19-01-all-the-places-for-patterns.html
  title: All the Places Patterns Can Be Used, The Rust Programming Language
  locator: while let Conditional Loops; Function Parameters
- url: https://peps.python.org/pep-0634/
  title: "PEP 634: Structural Pattern Matching: Specification"
  locator: Irrefutable case blocks; the header (Created 12-Sep-2020, Python-Version 3.10)
- url: https://blog.rust-lang.org/2022/11/03/Rust-1.65.0/
  title: Announcing Rust 1.65.0, The Rust Blog
  locator: let-else statements; the date (Nov. 3, 2022)
---

## Idea

A pattern tests a value and binds names. Some patterns cannot fail. The Rust Book states: "Patterns that will match for any possible value passed are irrefutable"[^1]. A single name, such as `x` in `let x = 5;`, is irrefutable, "because x matches anything and therefore cannot fail to match"[^1]. Other patterns can fail: "Patterns that can fail to match for some possible value are refutable"[^1]. `Some(x)` is refutable, because the value can be `None`.

The difference decides where a pattern can go. A binding has no place to go when its pattern fails, so it needs an irrefutable pattern. A test has an `else` branch, so it can take a refutable pattern. This Rust program uses both kinds:

```rust run id=idea
fn parse_pair(s: &str) -> Option<(i32, i32)> {
    // let-else: a refutable pattern, and an else block that returns.
    let Some((a, b)) = s.split_once(',') else {
        return None;
    };
    let (Ok(x), Ok(y)) = (a.trim().parse(), b.trim().parse()) else {
        return None;
    };
    Some((x, y))
}

fn main() {
    // let: an irrefutable tuple pattern.
    let point = (3, 4);
    let (x, y) = point;
    println!("x = {x}, y = {y}");

    // if let: a refutable pattern, and code for the match only.
    if let Some((a, b)) = parse_pair("5, 6") {
        println!("parsed {a} and {b}");
    }
    println!("{:?}", parse_pair("oops"));

    // while let: the loop runs while the pattern matches.
    let mut stack = vec![1, 2, 3];
    let mut popped = Vec::new();
    while let Some(top) = stack.pop() {
        popped.push(top);
    }
    println!("{popped:?}");
}
```

```output for=idea
x = 3, y = 4
parsed 5 and 6
None
[3, 2, 1]
```

## How it works

- **Which patterns are irrefutable.** The Rust Reference defines the two kinds: "A pattern is said to be refutable when it has the possibility of not being matched by the value it is being matched against. Irrefutable patterns, on the other hand, always match the value they are being matched against"[^2]. A name and the wildcard are irrefutable: "The wildcard pattern is always irrefutable"[^2]. A literal is refutable: "Literal patterns are always refutable"[^2]. A compound pattern takes its kind from its parts: "The tuple pattern is refutable when one of its subpatterns is refutable"[^2].
- **The type decides.** Refutability depends on the type of the value, not only on the pattern. A range pattern of an integer type is irrefutable when it covers the whole type: "For example, 0u8..=255u8 is irrefutable"[^2]. "Slice patterns are irrefutable when matching an array as long as each element is irrefutable"[^2], because the length of an array is part of its type. On a slice, the length is not known, so the same pattern can fail.
- **Bindings take irrefutable patterns.** "Function parameters, let statements, and for loops can only accept irrefutable patterns because the program cannot do anything meaningful when values don’t match"[^1]. The Rust compiler rejects a refutable pattern in a `let`, with error E0005: "Patterns used to bind names must be irrefutable, that is, they must guarantee that a name will be extracted in all cases"[^3].
- **Tests take refutable patterns.** "The if let and while let expressions and the let...else statement accept refutable and irrefutable patterns, but the compiler warns against irrefutable patterns"[^1], because a test that cannot fail is useless. A `match` takes refutable patterns in its arms, but the arms together must be exhaustive.
- **let-else.** A `let` statement can have an `else` block. The Rust Reference states: "If an else block is not present, the pattern must be irrefutable. If an else block is present, the pattern may be refutable"[^4]. "The else block must always diverge"[^4]: it must return, break, continue or panic, because the names of the pattern do not exist after a failed match.

## In languages

| Language | Where a pattern must not fail | Where a pattern can fail | A failed binding gives |
|---|---|---|---|
| Rust | `let`, function parameters, `for`[^1] | `match`, `if let`, `while let`, `let...else`[^1] | a compile-time error (E0005)[^3] |
| Python | an assignment such as `a, b = t`, a `for` target | `case`[^5] | an exception at run time |
| JavaScript, TypeScript | destructuring | none | `undefined` for a missing element; a `TypeError` for `null`[^6] |
| Java | none; patterns appear only in tests | `instanceof`, `switch`[^7] | no binding form |
| C# | a deconstruction, which "assigns the individual parts of a value"[^8] to variables | `is`, `switch` | no test; each part is assigned |
| Haskell | a pattern binding in `let` and `where`, made lazy[^9] | function equations, `case`[^9] | an error when a name is used |

- **Rust.** The compiler checks refutability for each place. A `let` with `Some(y)` gives "refutable pattern in local binding: `None` not covered"[^3].
- **Python.** Python checks a binding at run time. "Sequence unpacking requires that there are as many variables on the left side of the equals sign as there are elements in the sequence"[^10], and an assignment that breaks this rule raises an exception. In a `match`, the language reference names a match-all case: "A case block is considered irrefutable if it has no guard and its pattern is irrefutable"[^5], and "A match statement may have at most one irrefutable case block, and it must be last"[^5].
- **JavaScript.** Array destructuring does not fail when the array is short: "The values of the remaining variables will be undefined"[^6]. But "destructuring null or undefined throws a TypeError"[^6].
- **Java.** The JLS does not use the word irrefutable. It calls such a pattern unconditional: "A pattern p is said to be unconditional for a type T if it can be determined at compile time that every value of type T will match p"[^7].
- **Haskell.** The Haskell report also splits patterns into the two kinds. "Matching an irrefutable pattern is non-strict: the pattern matches even if the value to be matched is ⊥. Matching a refutable pattern is strict"[^9]. A pattern of the form `~apat` is irrefutable "whether or not apat is irrefutable"[^9]: the match is deferred until a name is used.

## Variations and related ideas

- **if let and exhaustiveness.** `if let` handles one pattern and ignores the others. The Rust Book warns: "you lose the exhaustive checking match enforces that ensures that you aren’t forgetting to handle any cases"[^11]. [[exhaustive-matching]] explains that check.
- **while let.** "the while let conditional loop allows a while loop to run for as long as a pattern continues to match"[^12]. A loop that pops a stack or reads from a channel ends when the pattern fails.
- **Destructuring.** A binding with an irrefutable pattern takes a value apart. [[destructure-a-value]] shows this in each language.
- **Lazy patterns.** In Haskell, "Pattern bindings are matched lazily; an implicit ~ makes these patterns irrefutable"[^9]. So a refutable pattern in a `let` does not fail when it is bound, but when a name from it is first used.

## Pitfalls

### A refutable pattern in a let

A `let` cannot handle a failed match, so Rust rejects a refutable pattern there:

```rust fails expect=compile id=rust-refutable-let
fn main() {
    let input: Option<i32> = Some(1);
    let Some(n) = input;
    println!("{n}");
}
```

Use `if let`, `match`, or `let...else` with a block that diverges. E0005 gives the same advice: "you probably need to use a match or if let"[^3].

### A binding that fails at run time

In Python, an assignment with several targets is a binding with no `else` branch. It fails at run time when the lengths differ:

```python run id=python-unpack-fails
def parse(line):
    key, value = line.split("=")
    return key, value


for line in ["width=80", "oops"]:
    try:
        print(parse(line))
    except ValueError as e:
        print(type(e).__name__)
```

```output for=python-unpack-fails
('width', '80')
ValueError
```

Use a `match` with a sequence pattern, which can fail and go on to the next case, or test the length first.

### An irrefutable pattern in a test

An irrefutable pattern in `if let` or `while let` always matches, so the `else` branch never runs and a `while let` never ends. Rust warns about it, because a pattern in these places is "intended to handle possible failure"[^1]. In Python, an irrefutable case before other cases is an error, because the case blocks after it can never run[^5].

### A let-else block that does not diverge

The `else` block of `let...else` must leave the current block, because "The else block must always diverge"[^4]. A block that only prints a message, and then goes on, does not compile. End it with `return`, `break`, `continue` or a panic.

## Cost

- **Bindings cost nothing extra.** An irrefutable pattern needs no test at run time. The JLS says of an unconditional pattern that "the run time, testing aspect of pattern matching could be elided"[^7].
- **Tests cost a test.** A pattern that is not unconditional keeps its test at run time, and each failed test sends control to the next arm or the `else` branch.

## History

- **Haskell 2010.** The Haskell report defines irrefutable and refutable patterns, and the `~apat` form that makes a pattern irrefutable[^9].
- **2020.** PEP 634, created on 12 September 2020 for Python 3.10, defined irrefutable case blocks: "A pattern is considered irrefutable if we can prove from its syntax alone that it will always succeed"[^13]. It also made the rule that such a case block must be last[^13].
- **2022.** Rust 1.65, released on 3 November 2022, added let-else. Before it, "Normal let statements can only use irrefutable patterns, statically known to always match"[^14]. "With let-else, a refutable pattern can match and bind variables in the surrounding scope like a normal let, or else diverge (e.g. break, return, panic!) when the pattern doesn't match"[^14].

## References
