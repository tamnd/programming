---
tag: ZVSAC3
kind: concept
slug: destructuring-assignment
name: Destructuring assignment
aliases: [destructuring, destructuring assignment, unpacking, tuple unpacking, iterable unpacking, parallel assignment, multiple assignment, deconstruction, pattern binding]
area: data-and-values/variables-and-assignment
level: core
summary: >-
  Destructuring assignment binds several names from the parts of one composite value, such as a tuple, an array or an object. The pattern on the left can skip parts, collect the rest and nest.
applies_to: core-languages
viz: none
formal: none
see_also: [assignment, variable-concept, single-assignment, binding, destructure-a-value, swap-two-values]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Destructuring
  title: Destructuring, MDN Web Docs
  locator: Description; Default value; Rest properties and rest elements
- url: https://doc.rust-lang.org/reference/patterns.html
  title: Patterns, The Rust Reference
  locator: patterns.destructure
- url: https://learn.microsoft.com/en-us/dotnet/csharp/fundamentals/patterns/deconstruct
  title: Deconstruct tuples and other types, C# guide
  locator: Introduction
- url: https://docs.python.org/3/reference/simple_stmts.html
  title: Simple statements, The Python Language Reference
  locator: 7.2 Assignment statements
- url: https://en.wikipedia.org/wiki/Assignment_(computer_science)
  title: Assignment (computer science), Wikipedia
  locator: Parallel assignment
- url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html
  title: Chapter 14, Blocks, Statements, and Patterns, The Java Language Specification, Java SE 25
  locator: 14.30.1 Kinds of Patterns
- url: https://en.cppreference.com/w/cpp/language/structured_binding.html
  title: Structured binding declaration, cppreference.com (C++)
  locator: Introduction
---

## Idea

A value often has parts: a point has `x` and `y`, a row has a name and some scores. Destructuring takes the parts out into names in one step. "The destructuring syntax is a JavaScript syntax that makes it possible to unpack values from arrays, or properties from objects, into distinct variables."[^1] In Python it is called unpacking:

```python run id=idea
point = (3, 4)
x, y = point
print(x, y)

name, *scores = ["Ada", 90, 85, 77]
print(name, scores)
```

```output for=idea
3 4
Ada [90, 85, 77]
```

Without destructuring, the program reads each part with an index: `x = point[0]` and `y = point[1]`.

## How it works

**A pattern on the left side.** The left side of the assignment has the shape of the value. Rust says: "Destructuring breaks up a value into its component pieces. The syntax used is almost the same as when creating such values."[^2] A tuple pattern `(a, b)` takes a pair apart, and an object pattern `{ name, age }` takes two properties.

**By position or by name.** Tuples and arrays are taken apart by position: the first name gets the first item. Objects, structs and records are taken apart by name: JavaScript `const { width, height } = box` reads the properties `width` and `height`, in any order. In C#, "A deconstruction assigns the individual parts of a value — its components — to multiple variables in a single operation."[^3] A class can define a `Deconstruct` method to say what its parts are.

**The count must match.** In Python, a pattern with a starred name takes the other items: "The object must be an iterable with at least as many items as there are targets in the target list, minus one."[^4] Without a starred name, the counts must be equal, and `a, b = [1, 2, 3]` raises `ValueError`. JavaScript does not check: a missing item gives `undefined`, and extra items are ignored.

**Parallel assignment.** Destructuring of a tuple that the program writes on the right side assigns several names at once. Many languages allow it, such as APL, Common Lisp, Go, JavaScript, Julia, PHP, Lua, Perl, Python, Ruby and PowerShell[^5]. The right side is computed first, so `a, b = b, a` swaps two values.

**Defaults, skips, rest and nesting.** In JavaScript, "Each destructured property can have a default value. The default value is used when the property is not present, or has value undefined."[^1] A name such as `_`, or an empty place in `[a, , c]`, skips a part. A rest name collects the other items: "For array destructuring, it collects remaining elements of the iterable into a new array"[^1]. A pattern can contain other patterns, such as `[[a, b], c]`.

**Patterns in other places.** The same patterns appear in parameters, in `for` loops such as `for (key, value) in pairs`, and in `match` or `switch`. Java has record patterns in `instanceof` and `switch`: "A record pattern consists of a ReferenceType and a component pattern list containing component patterns, if any."[^6] C++ has structured bindings, which "Binds the specified names to subobjects or elements of the initializer"[^7].

## In languages

| Language | Verdict | Destructuring |
|---|---|---|
| Python | native | `a, b = pair`, `first, *rest = items`, nested targets, and patterns in `match` |
| JavaScript | native | array patterns `[a, b]` and object patterns `{ a, b }`, with defaults, renames and rest |
| TypeScript | native | the patterns of JavaScript, with types |
| Java | native | record patterns in `instanceof` and `switch` |
| Kotlin | native | `val (a, b) = pair` with `componentN` functions, for data classes and pairs |
| C# | native | `var (a, b) = tuple`, `Deconstruct` methods, and discards `_` |
| Go | idiom | `a, b := f()` for several return values, and parallel assignment `a, b = b, a`; no patterns for structs or slices |
| Rust | native | patterns in `let`, `match`, `if let` and parameters, for tuples, structs, enums and slices |
| C | absent | no destructuring; read each member of a struct |
| C++ | native | structured bindings `auto [a, b] = pair` |
| Bash | idiom | `read -r a b <<< "$line"` splits a line into names |
| PowerShell | native | `$a, $b = 1, 2`, and the last name collects the rest |
| SQL | n/a | a query selects columns by name, with no assignment of parts |
| Regex | idiom | capture groups take the parts of a match, and named groups give them names |
| HTML | n/a | HTML is markup, with no variables |
| CSS | n/a | CSS has no variables to bind |

**C++.** A structured binding is an alias, not a copy: "Like a reference, a structured binding is an alias to an existing object."[^7] With `auto&`, a change through the name changes the original object.

## Variations and related ideas

- **Swap.** `a, b = b, a` in Python and `[a, b] = [b, a]` in JavaScript swap two values without a temporary variable.
- **Several return values.** A function returns a tuple, and the caller destructures it: `q, r = divmod(17, 5)`.
- **Pattern matching.** Destructuring is the binding part of pattern matching. A `match` also tests the shape and picks a branch.
- **Ignore parts.** `_` takes a part and drops it, in Python, Go, Rust, C# and other languages.

## Pitfalls

- **A default for null.** In JavaScript, a default value replaces `undefined` but not `null`. "It is not used if the property has value null."[^1]

```javascript wrong id=default-and-null
const { size = 10 } = { size: null };
console.log(size);
const { limit = 10 } = {};
console.log(limit);
```

```output for=default-and-null
null
10
```

Use `const size = options.size ?? 10;` when `null` also means "no value".

- **A count that does not match.** Python raises `ValueError` when the counts differ. JavaScript and Lua fill the gap with `undefined` or `nil`, so the error shows up later.
- **A copy or an alias.** In C++, `auto [a, b] = p` copies `p` and binds names to the copy, but `auto& [a, b] = p` binds names to `p` itself. In Rust and JavaScript, destructuring of an object copies or moves its parts.
- **A statement that starts with a brace.** In JavaScript, `{ a, b } = obj;` without `const` or `let` is a block, not a pattern. Write `({ a, b } = obj);`.

## Cost

Destructuring costs the same as a read of each part. In a compiled language, `auto [q, r] = f()` is the same code as two reads of the members. Python builds no new objects for the targets, but a starred name makes a new list.

## History

Parallel assignment is old: "it was introduced in CPL in 1963, under the name simultaneous assignment"[^5]. Later, many languages added destructuring of tuples, arrays and objects. C++ added structured bindings in C++17[^7].

## References
