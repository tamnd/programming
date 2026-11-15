---
tag: 0MTSVJ
kind: concept
slug: for-each-loop
name: For-each loop
aliases: [for-each loop, foreach loop, foreach, enhanced for loop, for-in loop, for...of, range-based for loop, for range, enumeration loop, collection-controlled loop, iterator loop]
area: errors-and-control/loops
level: intro
summary: >-
  A for-each loop runs its body once for each element of a collection, with no index to manage. Under it, the language asks the collection for an iterator and calls it until it has no more elements.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2004
  note: Java 5, released in September 2004, added the enhanced for statement (JSR 201). Perl and C# call the loop foreach.
see_also: [loop, for-loop, iterators, iterator-invalidation, while-loop, loop-and-a-half, higher-order-functions, array]
same_as:
  wikidata: Q3273595
  wikipedia: Foreach_loop
sources:
- url: https://en.wikipedia.org/wiki/Loop_(statement)
  title: Loop (statement), Wikipedia
  locator: Enumeration
- url: https://docs.python.org/3/reference/compound_stmts.html
  title: Compound statements, The Python Language Reference
  locator: 8.3 The for statement
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...of
  title: for...of, MDN Web Docs
  locator: Description; Difference between for...of and for...in
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: The Java Language Specification, Java SE 25 Edition, Chapter 14
  locator: 14.14.2 The enhanced for statement
- url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/iteration-statements
  title: Iteration statements, C# language reference
  locator: The foreach statement
- url: https://doc.rust-lang.org/reference/expressions/loop-expr.html
  title: Loops and other breakable expressions, The Rust Reference
  locator: Iterator loops
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://en.cppreference.com/w/cpp/language/range-for.html
  title: Range-based for loop, C++ reference, cppreference.com
  locator: Introduction; Temporary range initializer
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Statements/for...in
  title: for...in, MDN Web Docs
  locator: Introduction; Array iteration and for...in
- url: https://docs.python.org/3/tutorial/controlflow.html
  title: More Control Flow Tools, The Python Tutorial
  locator: 4.2 for Statements
- url: https://en.wikipedia.org/wiki/Java_version_history
  title: Java version history, Wikipedia
  locator: J2SE 5.0
---

## Idea

Most loops visit each element of a list, a set, a map or a string. A [for loop](/for-loop/) does this with an index: start at 0, read `a[i]`, step `i`, stop at the length. A for-each loop leaves out the index. It names a variable, names the collection, and runs the body once with each element in the variable.

```python run id=idea
colors = ["red", "green", "blue"]
for color in colors:
    print(color)

# enumerate gives the index too, when the body needs it.
for i, color in enumerate(colors):
    print(i, color)

# A dictionary gives its keys; items() gives the pairs.
ages = {"ann": 31, "bob": 27}
for name, age in ages.items():
    print(name, age)

# What the for statement does: get an iterator, call next until StopIteration.
it = iter(colors)
while True:
    try:
        color = next(it)
    except StopIteration:
        break
    print("by hand:", color)
```

```output for=idea
red
green
blue
0 red
1 green
2 blue
ann 31
bob 27
by hand: red
by hand: green
by hand: blue
```

The first loop has no index and no length, so it cannot read past the end or skip an element. `enumerate` adds an index for the cases that need one. A dictionary loop can take the key and the value apart in the loop header. The last loop does by hand what the `for` statement does: it gets an iterator from the list and asks it for the next element until it raises `StopIteration`.

## How it works

**A loop over elements, not positions.** Wikipedia calls this form an enumeration, "a loop intended to iterate over all the items of a collection"[^1]. It is "not as flexible as a conditional loop; but it is more predictable"[^1]: it ends when the elements end.

**The iterator protocol under it.** Each language turns a for-each loop into calls on an iterator. See [iterators](/iterators/).

- Python: the collection "is evaluated once; it should yield an iterable object. An iterator is created for that iterable"[^2], and the loop ends when the iterator is exhausted.
- JavaScript: `for...of` "first calls the iterable's [Symbol.iterator]() method, which returns an iterator, and then repeatedly calls the resulting iterator's next() method to produce the sequence of values"[^3]. Arrays, strings, maps, sets and generators are iterable.
- Java: in the enhanced `for`, "The type of the Expression must be an array type (§10.1) or a subtype of the raw type Iterable, or a compile-time error occurs"[^4].
- C#: `foreach` works "for each element in an instance of the type that implements the System.Collections.IEnumerable or System.Collections.Generic.IEnumerable<T> interface"[^5]. A type with a `GetEnumerator` method also works, and the enumerator has "the public Current property and the public parameterless MoveNext method whose return type is bool"[^5].
- Rust: "A for expression is a syntactic construct for looping over elements provided by an implementation of std::iter::IntoIterator"[^6].
- Go: a `for` with a `range` clause "iterates through all entries of an array, slice, string or map, values received on a channel, integer values from zero to an upper limit [Go 1.22], or values passed to an iterator function's yield function [Go 1.23]"[^7].
- C++: the range-based `for` is "a more readable equivalent to the traditional for loop operating over a range of values, such as all elements in a container"[^8]. It calls `begin` and `end` on the range.

**The names.** "Enumerations are sometimes called "for loops," for example in Zig and Rust"[^1]. This can be confusing, because C, C++ and Java "use that term for the three-part for loop"[^1]. "Other programming languages, such as Perl and C#, avoid this confusion by using the term "foreach loop.""[^1].

**`for...in` is not `for...of` in JavaScript.** "The for...in statement iterates over the enumerable string properties of an object, while the for...of statement iterates over values that the iterable object defines to be iterated over"[^3]. On an array, `for...in` gives the indexes as strings, and "the for...in loop will return all enumerable properties, including those with non–integer names"[^9]. Use `for...of` for the elements.

**The order.** "The order in which the items in the collection are iterated through depends on the programming language"[^1] and on the collection. A list gives its elements in order. In Go, "The iteration order over maps is not specified and is not guaranteed to be the same from one iteration to the next"[^7], so a program that needs a fixed order sorts the keys first.

**The loop variable.** In most languages the loop variable gets a copy of the element or a reference to it, and the difference matters when the body changes it. Python assigns each element to the name, and an assignment to the name in the body does not change the list. Go copies each element of a slice into the variable. C++ copies with `for (auto x : v)` and refers to the element with `for (auto& x : v)`. Rust moves the elements out of a vector with `for x in v`, borrows them with `for x in &v` and borrows them for change with `for x in &mut v`.

**Changing the collection in the loop.** The Python tutorial warns that "Code that modifies a collection while iterating over that same collection can be tricky to get right"[^10], and advises a loop over a copy or the building of a new collection. Java and C# throw an exception when a list changes during a for-each loop, Python raises an error when a dictionary changes size, and in C++ the iterator can become invalid, which is undefined behavior.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | `for x in iterable:`; `enumerate`, `zip` and `dict.items()`; the `__iter__` and `__next__` protocol |
| JavaScript | native | `for (const x of iterable)` over the `Symbol.iterator` protocol; `for...in` over property names |
| TypeScript | native | `for...of` of JavaScript; the element type comes from the iterable type |
| Java | native | the enhanced `for (T x : expr)` over an array or an `Iterable` |
| Kotlin | native | `for (x in collection)` over anything with an `iterator()` operator; `withIndex()` |
| C# | native | `foreach (var x in e)` over `IEnumerable<T>` or a `GetEnumerator` pattern; `await foreach` |
| Go | native | `for i, v := range x` over arrays, slices, strings, maps, channels, integers and iterator functions |
| Rust | native | `for x in expr` over `IntoIterator`; `&v`, `&mut v` and `v` borrow, change or move |
| C | absent | no for-each loop; an index or pointer loop with a known length or a sentinel |
| C++ | native | the range-based `for (auto& x : range)` over `begin` and `end` |
| Bash | native | `for x in words`; `"${array[@]}"` loops over the elements of an array |
| PowerShell | native | `foreach ($x in $items)` and the `ForEach-Object` cmdlet |
| SQL | idiom | a query works on each row of a set with no loop; cursors loop over rows in procedural code |
| Regex | n/a | no collections |
| HTML | n/a | no loops |
| CSS | n/a | no loops; a selector applies a rule to each matching element |

All the general-purpose languages except C have a for-each loop, and they differ in the protocol under it and in what the loop variable holds. Java, C#, Kotlin, Python and JavaScript give a reference to each object, C++ and Go give a copy unless the code asks for more, and Rust makes the choice of move, borrow or change a part of the loop header.

The example files show these differences. Python loops over a dictionary and gets an error when the dictionary changes size. JavaScript compares `for...of` with `for...in` and loops over a `Map` and over the code points of a string. Java loops over an array and gets a `ConcurrentModificationException` from a list. C# takes tuples apart in the loop header, loops over an iterator method and gets an exception when a list changes. Go shows that the range variable is a copy, sorts map keys for a fixed order and loops over the runes of a string. Rust borrows, changes and moves a vector. C has no for-each loop and loops with a length or a sentinel. C++ uses references, structured bindings over a map and an init statement.

## Variations and related ideas

- **The index.** When the body needs the position too, use `enumerate` in Python, `entries()` in JavaScript, `withIndex()` in Kotlin, `.iter().enumerate()` in Rust or the first value of `range` in Go.
- **Several collections at once.** `zip` in Python and Rust gives pairs from two collections, and stops at the end of the shorter one.
- **Destructuring.** Python, JavaScript, C#, Rust and C++ can take a pair or a tuple apart in the loop header.
- **For-each methods.** `forEach` in JavaScript and Java, and `for_each` in Rust, take a function and call it for each element. A `break` does not work in them. See [higher-order functions](/higher-order-functions/).
- **Asynchronous loops.** `for await...of` in JavaScript and `await foreach` in C# wait for each element of an asynchronous stream.

## Pitfalls

- **Changing the collection.** Adding or removing elements during the loop raises an error or skips elements. Loop over a copy, collect the changes and apply them after the loop, or use a method such as `removeIf`. See [iterator invalidation](/iterator-invalidation/).
- **A copy instead of the element.** In Go and in C++ with `auto`, a change to the loop variable changes a copy, and the collection stays the same. Use the index in Go and `auto&` in C++.
- **`for...in` on arrays in JavaScript.** It gives the keys as strings, and it includes other enumerable properties. Use `for...of`.
- **Map order.** Do not depend on the order of a hash map. Sort the keys when the output must be stable.
- **A moved collection in Rust.** `for x in v` moves `v`, so the code after the loop cannot use `v`. Loop over `&v` to keep it.
- **Temporaries in C++.** In the range-based `for`, "If range-initializer returns a temporary, its lifetime is extended until the end of the loop"[^8], but in older C++ standards a temporary deeper in the expression, as in `for (auto& x : make().items())`, is destroyed before the body runs.

## Cost

A for-each loop over an array or a vector compiles to the same code as an index loop in C++, Rust, Go, Java and C#, and compilers can remove bounds checks because the loop never leaves the collection. A loop over a linked list or a tree costs what its iterator costs. In Python, `for x in xs` is faster and clearer than `for i in range(len(xs))` with `xs[i]` in the body, because it does no index arithmetic.

## History

Many language families have a loop over the items of a collection. Wikipedia notes that "descendants of ALGOL use for", that "descendants of Fortran use do" and that "COBOL uses PERFORM VARYING"[^1]. Perl and C# called it `foreach`. Java 5, released on 30 September 2004, added the "Enhanced for each loop", with "special syntax for iterating over each member of either an array or any Iterable"[^11].

## References
