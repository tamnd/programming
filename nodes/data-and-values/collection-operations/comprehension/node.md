---
tag: NYN64N
kind: concept
slug: comprehension
name: Comprehension
aliases: [list comprehension, set comprehension, dict comprehension, dictionary comprehension, generator expression, for comprehension, query expression, query syntax, set-builder notation, for-yield]
area: data-and-values/collection-operations
level: core
summary: >-
  A comprehension is an expression that builds a collection from a source, an optional filter and an expression for each item. It does the work of a loop that appends to a new list.
applies_to: core-languages
viz: optional
formal: none
see_also: [map-over-a-list, filter-a-list, flatten-a-nested-list, compute-the-cartesian-product, iterators, higher-order-functions, replace-loop-with-pipeline, reduce-a-list-to-a-value]
sources:
- url: https://docs.python.org/3/tutorial/datastructures.html
  title: Data Structures, The Python Tutorial
  locator: 5.1.3 List Comprehensions
- url: https://en.wikipedia.org/wiki/Set-builder_notation
  title: Set-builder notation, Wikipedia
  locator: Introduction
- url: https://docs.python.org/3/reference/expressions.html
  title: Expressions, The Python Language Reference
  locator: 6.2.4 Displays for lists, sets and dictionaries; 6.2.7 Generator expressions
- url: https://peps.python.org/pep-0202/
  title: PEP 202, List Comprehensions
  locator: Rationale
- url: https://www.haskell.org/onlinereport/haskell2010/haskellch3.html
  title: Expressions, Haskell 2010 Language Report
  locator: 3.11 List Comprehensions
- url: https://www.erlang.org/doc/system/list_comprehensions.html
  title: List Comprehensions, Erlang System Documentation
  locator: Simple Examples
- url: https://peps.python.org/pep-0274/
  title: PEP 274, Dict Comprehensions
  locator: Abstract
- url: https://peps.python.org/pep-0289/
  title: PEP 289, Generator Expressions
  locator: Abstract; Rationale
- url: https://peps.python.org/pep-0572/
  title: PEP 572, Assignment Expressions
  locator: Scope of the target
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/get-started/write-linq-queries
  title: Write C# LINQ queries to query data, Microsoft Learn
  locator: Introduction
- url: https://docs.scala-lang.org/tour/for-comprehensions.html
  title: For Comprehensions, Tour of Scala
  locator: Introduction
- url: https://hexdocs.pm/elixir/comprehensions.html
  title: Comprehensions, Elixir documentation
  locator: Introduction; Generators and filters
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
  title: Package java.util.stream, Java SE 25 API
  locator: Stream operations and pipelines
- url: https://doc.rust-lang.org/std/iter/index.html
  title: Module iter, The Rust Standard Library
  locator: Laziness
- url: https://en.cppreference.com/w/cpp/ranges.html
  title: Ranges library, cppreference.com
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/get-started/query-expression-basics
  title: Query expression basics, Microsoft Learn
  locator: What is a query expression?; Query variable
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/flatMap
  title: Array.prototype.flatMap(), MDN Web Docs
  locator: Description
- url: https://go.dev/ref/spec
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://peps.python.org/pep-0709/
  title: PEP 709, Inlined comprehensions
  locator: Abstract
- url: https://kotlinlang.org/docs/collection-transformations.html
  title: Transform collections, Kotlin documentation
  locator: Map
- url: https://en.wikipedia.org/wiki/List_comprehension
  title: List comprehension, Wikipedia
  locator: History
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator
  title: Iterator, MDN Web Docs
  locator: Iterator helper methods
---

## Idea

A program often makes a new collection from an old one: it takes each item, keeps some of them and computes a new value from each item that it keeps. A loop can do this work. The loop makes an empty list, reads the source, tests each item and appends the result. A comprehension writes the same work as one expression. The Python tutorial says: "List comprehensions provide a concise way to create lists"[^1].

A comprehension has three parts:

- a **source**, the collection or iterator that gives the items;
- an optional **filter**, a condition that each item must pass;
- an **expression**, which gives the value to put in the result for each item that passes.

The form comes from mathematics. Set-builder notation writes a set as `{ 2n | n ∈ ℕ, n < 5 }`, which is the set of all 2n where n is a natural number less than 5. Wikipedia defines it as "a notation for specifying a set by a property that characterizes its members"[^2]. A comprehension in a programming language uses the same shape, but its source is a finite collection or a stream, so a computer can build the result.

```python run id=idea
words = ["apple", "fig", "banana", "kiwi", "cherry"]

# A list comprehension: an expression, a source and a filter.
lengths = [len(w) for w in words if len(w) > 3]
print(lengths)

# The same list, built with a loop.
result = []
for w in words:
    if len(w) > 3:
        result.append(len(w))
print(result == lengths)

# Set and dict comprehensions use braces.
print(sorted({w[0] for w in words}))
print({w: len(w) for w in words if "i" in w})

# Two for clauses nest like two loops. The first for clause is the outer loop.
print([(x, y) for x in range(1, 3) for y in "ab"])

# A generator expression makes the items one at a time, when they are needed.
squares = (n * n for n in range(1_000_000))
print(next(squares), next(squares), sum(n * n for n in range(4)))

# The loop variable does not leak out of the comprehension.
w = "outer"
copy = [w for w in words]
print(w)
```

```output for=idea
[5, 6, 4, 6]
True
['a', 'b', 'c', 'f', 'k']
{'fig': 3, 'kiwi': 4}
[(1, 'a'), (1, 'b'), (2, 'a'), (2, 'b')]
0 1 14
outer
```

The first comprehension and the loop give the same list. The braces make a set or a dict. Two `for` clauses give all the pairs, in the order of two nested loops. The generator expression does not make a million squares. It makes each square when `next` or `sum` asks for it. After the comprehension, `w` still has its old value.

## How it works

A comprehension is a short way to write a loop. The Python reference describes the parts: "In its simplest form, a comprehension consists of a single expression followed by a for clause"[^3], and the tutorial gives the full form: "A list comprehension consists of brackets containing an expression followed by a for clause, then zero or more for or if clauses. The result will be a new list resulting from evaluating the expression in the context of the for and if clauses which follow it"[^1].

To read a comprehension with many clauses, write each clause on its own line in the same order, as a loop or an `if` inside the clause before it, and put the expression at the innermost level with an append. So `[x for row in m if row for x in row]` is the same as:

```text
result = []
for row in m:
    if row:
        for x in row:
            result.append(x)
```

The expression comes first in the text, but it runs last, once for each set of values that the clauses produce.

The same idea has other names in other languages:

- A **map and a filter.** PEP 202 gives the reason for list comprehensions in Python: "List comprehensions provide a more concise way to create lists in situations where map() and filter() and/or nested loops would currently be used"[^4]. A comprehension with one `for` and one `if` is a filter followed by a map. A comprehension with two `for` clauses is a flat map. The Haskell report defines a list comprehension by a translation into simpler expressions, and the translation uses `concatMap`[^5]. See [map over a list](/map-over-a-list/), [filter a list](/filter-a-list/) and [flatten a nested list](/flatten-a-nested-list/).
- **Generators and qualifiers.** The Haskell report says: "A list comprehension has the form [ e | q1, …, qn ],n ≥ 1, where the qi qualifiers are either generators", boolean guards or local bindings. It also gives the order of the work: "Such a list comprehension returns the list of elements produced by evaluating e in the successive environments created by the nested, depth-first evaluation of the generators in the qualifier list"[^5]. Erlang uses the same words. Its guide reads `[X || X <:- [1,2,a,3,4,b,5,6], X > 3]` like set-builder notation: "The list of X such that X is taken from the list [1,2,a,...] and X is greater than 3"[^6].
- **Other result types.** The result need not be a list. Python has set comprehensions and dict comprehensions. PEP 274 calls the second form "the “dictionary comprehension” or “dict comprehension” for short"[^7]. A dict comprehension has a key expression and a value expression.
- **Lazy results.** A generator expression has the same syntax in parentheses, but it gives an iterator, not a list. PEP 289 describes "generator expressions as a high performance, memory efficient generalization of list comprehensions"[^8]. The Python reference says that "a generator expression evaluates to a generator iterator which yields the same values as the corresponding list comprehension"[^3].

**Scope.** In Python, the names in the `for` clauses belong to the comprehension: "the comprehension is executed in a separate implicitly nested scope. This ensures that names assigned to in the target list don’t “leak” into the enclosing scope"[^3]. There are two exceptions. First, "The iterable expression in the leftmost for clause is evaluated directly in the enclosing scope and then passed as an argument to the implicitly nested scope"[^3], so the first source is evaluated at once, also for a generator expression. Second, an assignment expression with `:=` inside a comprehension "binds the target in the containing scope"[^9]. PEP 572 gives a reason: the name can keep a witness for `any()` or a counterexample for `all()`.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | native | list `[e for x in s if c]`, set `{e for ...}`, dict `{k: v for ...}` and generator `(e for ...)` comprehensions |
| JavaScript | idiom | no comprehension syntax; `filter`, `map`, `flatMap`, `Array.from` and the lazy iterator helpers |
| TypeScript | idiom | the JavaScript methods, with types for the items and the result |
| Java | stdlib | streams: `filter`, `map`, `flatMap`, then `toList` or a `Collector` |
| Kotlin | stdlib | `filter`, `map`, `flatMap` and `associate` on collections; `sequence` for lazy work |
| C# | native | LINQ query syntax `from x in s where c select e`; the compiler turns it into `Where` and `Select` calls |
| Go | idiom | no comprehension syntax; a `for` loop with `append`, or an iterator function in a `range` loop |
| Rust | stdlib | iterator adapters `filter`, `map` and `flat_map`, then `collect` into the type that the program names |
| C | idiom | a `for` loop that writes into an array and counts the items |
| C++ | stdlib | range views `std::views::filter` and `std::views::transform`, joined with `\|` |
| Bash | idiom | a `for` loop over words with a test, or a pipeline of commands such as `grep` and `sed` |
| PowerShell | idiom | a pipeline with `Where-Object` and `ForEach-Object` |
| SQL | native | `SELECT e FROM t WHERE c` has a source, a filter and an expression for each row |
| Regex | n/a | a pattern matches text and does not build collections |
| HTML | n/a | markup does not build collections |
| CSS | n/a | the style rules do not build collections |

A comprehension is syntax, so only some languages have it. Other languages give the same result with higher-order functions, and the chain of calls has the same three parts. Many languages that have comprehensions also define them as a translation into such calls. C# says: "The C# compiler translates query syntax into method calls"[^10]. Scala says: "Comprehensions have the form for (enumerators) yield e" and "A comprehension evaluates the body e for each binding generated by the enumerators and returns a sequence of these values"[^11]. Elixir says: "Comprehensions are syntactic sugar for such constructs" and "A comprehension is made of three parts: generators, filters, and collectables"[^12].

The example files show these forms. Python shows a flatten, a nested comprehension, the difference between a filter and a conditional expression, an assignment expression that leaves the comprehension, and the order in which a generator expression evaluates its sources. JavaScript shows `filter` and `map`, a `Set`, `Object.fromEntries`, `flatMap` and the lazy iterator helpers. Java shows a stream with `toList`, `toMap`, `flatMap` and the laziness of `limit`. C# shows query syntax, two `from` clauses, the same query in method syntax and a deferred query. Go shows loops, a set made from a map and an iterator function. Rust shows `collect` into a vector, a set and a map, `flat_map` and the laziness of `take`. C shows the loop that a comprehension replaces. C++ shows a view pipeline and that a view runs only when the program reads it.

## Variations and related ideas

- **Eager and lazy.** A list comprehension builds the whole result at once. A generator expression, a Java stream, a Rust iterator chain, a C++ view and a C# query make the items when the program reads them. The Java documentation says that intermediate operations "are always lazy; executing an intermediate operation such as filter() does not actually perform any filtering"[^13]. Rust says: "Iterators (and iterator adapters) are lazy. This means that just creating an iterator doesn’t do a whole lot. Nothing really happens until you call next"[^14]. C++ says that range adaptors "are applied to views lazily. Adaptors can be composed into pipelines, so that their actions take place as the view is iterated"[^15].
- **Deferred queries.** A lazy query runs again each time the program reads it, so it sees changes to its source. C# says: "A query is distinct from the results that it produces"[^16], and a query variable "is always an enumerable type that produces a sequence of elements when iterated over"[^16]. The C# example adds an item to the list after it makes the query, and the query result contains the new item.
- **Flat map.** Two generators in one comprehension do the same work as a flat map. MDN says that `flatMap` "is identical to a map() followed by a flat() of depth 1"[^17]. See [compute the Cartesian product](/compute-the-cartesian-product/).
- **Collection pipelines.** A long chain of `filter`, `map`, `sort` and `reduce` calls is a collection pipeline. A comprehension covers only the map and the filter, so a pipeline is the more general form. See [replace loop with pipeline](/replace-loop-with-pipeline/) and [reduce a list to a value](/reduce-a-list-to-a-value/).
- **Iterator functions.** Go has no comprehension, but a `range` loop accepts a function that calls `yield` for each value, so a function can produce values one at a time[^18]. Python generators and JavaScript generator functions do the same with `yield`. See [iterators](/iterators/).

## Pitfalls

- **Too much in one expression.** A comprehension with three `for` clauses, two filters and a conditional expression is hard to read. Use a loop or a named function when the comprehension does not fit on one or two lines.
- **The filter and the conditional expression.** In Python, `[x for x in s if c]` drops the items that fail `c`, but `[x if c else y for x in s]` keeps every item and changes some of them. The `if` after the `for` is a filter, and the `if` before the `for` is part of the expression.
- **The order of the clauses.** The first `for` clause is the outer loop. To flatten a list of rows, write `[x for row in m for x in row]`. The other order uses `row` before it has a value.
- **Side effects.** A comprehension that runs only for its side effects, such as `[print(x) for x in s]`, builds a list that the program does not use. Rust gives a warning for a lazy chain that is never used, because "iterators are lazy and do nothing unless consumed"[^14]. Use a loop.
- **A lazy result used twice.** A generator expression or a Java stream can be read only once. The second read of a Python generator gives no items. The Java documentation says: "The elements of a stream are only visited once during the life of a stream. Like an Iterator, a new stream must be generated to revisit the same elements of the source"[^13]. Make a list when the program needs the items more than once.
- **A deferred query and a changing source.** A lazy query reads its source when the program reads the query, not when the program makes it. If the source changes between the two, the result changes too. Make a list at once when the program needs a snapshot.
- **The first source runs at once.** A Python generator expression evaluates its leftmost source when the program makes it, and evaluates the rest later. An error in the first source appears at once, and an error in the other parts appears when the program reads the items.

## Cost

A comprehension does the same work as the loop that it replaces: the expression runs once for each item that passes the filter, and the filter runs once for each item of the source. A list, set or dict comprehension uses memory for the whole result. A generator expression uses memory for one item at a time. PEP 289 gives an example: "Memory is conserved by using a generator expression instead: sum(x*x for x in range(10))"[^8].

CPython first compiled each comprehension as a nested function. PEP 709 says: "Comprehensions are currently compiled as nested functions, which provides isolation of the comprehension’s iteration variable, but is inefficient at runtime. This PEP proposes to inline list, dictionary, and set comprehensions into the code where they are defined"[^19]. Kotlin `map` on a list "applies the given lambda function to each subsequent element and returns the list of the lambda results"[^20], so a chain of eager calls makes a new list at each step. A lazy chain of adapters, such as a Rust iterator chain, makes no lists between the steps.

## History

Set-builder notation came first, in set theory. Wikipedia notes that "Specifying sets by member properties is allowed by the axiom schema of specification. This is also known as set comprehension and set abstraction"[^2].

Wikipedia gives this history for programming languages[^21]. "The SETL programming language (1969) has a set formation construct which is similar to list comprehensions." Rod Burstall and John Darlington used the word comprehension for this construct in their description of the functional language NPL in 1977, and David Turner recalls that "it was Phil Wadler who coined the better term list comprehension". Turner's lazy functional language Miranda had list comprehensions, and "The subsequently developed standard pure lazy functional language Haskell includes many of Miranda's features, including list comprehensions." Wikipedia also says that "Comprehensions were proposed as a query notation for databases".

Python added list comprehensions with PEP 202[^4], dict comprehensions with PEP 274[^7] and generator expressions with PEP 289[^8]. C# added query expressions with LINQ. They use the keywords `from`, `where` and `select`[^16]. Languages that do not have comprehension syntax, such as Java, Rust, JavaScript and Kotlin, added lazy or eager chains of `filter` and `map` to their standard libraries[^13][^14][^22][^20].

## References
