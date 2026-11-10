---
tag: NEW-iterators
kind: concept
slug: iterators
name: Iterator
aliases: [enumerator, iteration protocol]
area: functions-and-abstraction/generators-and-coroutines
level: core
summary: >-
  An iterator gives the elements of a sequence one at a time and remembers its position between calls. A loop asks it for the next element until it says that no elements are left.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1977
  people: [Barbara Liskov, Russ Atkinson]
  note: CLU had iterators, procedures that yield elements one at a time to the body of a for statement. Russ Atkinson designed them in 1975, after the CLU group learned of the generators of Alphard.
see_also: [closures, exception-handling, parametric-polymorphism, recursion, async-await]
same_as:
  wikidata: Q1326388
  wikipedia: Iterator
sources:
  - url: https://peps.python.org/pep-0234/#python-api-specification
    title: "PEP 234: Iterators, Ka-Ping Yee and Guido van Rossum"
    locator: Python API Specification
  - url: https://docs.python.org/3.14/library/stdtypes.html#iterator-types
    title: The Python Standard Library, Built-in Types, Python 3.14
    locator: Iterator Types, iterator.__iter__ and iterator.__next__
  - url: https://docs.python.org/3.14/library/stdtypes.html#dict-views
    title: The Python Standard Library, Built-in Types, Python 3.14
    locator: Dictionary view objects
  - url: https://docs.python.org/3.14/glossary.html#term-iterator
    title: Python Glossary, Python 3.14
    locator: iterator
  - url: https://docs.python.org/3.14/reference/compound_stmts.html#the-for-statement
    title: The Python Language Reference, Compound statements, Python 3.14
    locator: 8.3 The for statement
  - url: https://docs.python.org/3.14/tutorial/controlflow.html#for-statements
    title: The Python Tutorial, More Control Flow Tools, Python 3.14
    locator: 4.2 for Statements
  - url: https://tc39.es/ecma262/2026/#sec-iterable-interface
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 27.1.1.1 The Iterable Interface
  - url: https://tc39.es/ecma262/2026/#sec-iterator-interface
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 27.1.1.2 The Iterator Interface; 27.1.1.5 The IteratorResult Interface
  - url: https://tc39.es/ecma262/2026/#sec-iteratorclose
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 7.4.11 IteratorClose
  - url: https://tc39.es/ecma262/2026/#sec-iterator-helper-objects
    title: ECMA-262, ECMAScript 2026 Language Specification
    locator: 27.1.2 Iterator Helper Objects
  - url: https://262.ecma-international.org/6.0/#sec-iteration
    title: ECMA-262, 6th Edition, ECMAScript 2015 Language Specification, June 2015
    locator: 25.1 Iteration
  - url: https://tc39.es/ecma262/2025/multipage/control-abstraction-objects.html#sec-iterator.prototype.map
    title: ECMA-262, ECMAScript 2025 Language Specification
    locator: Iterator.prototype.map
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ArrayList.html
    title: Java SE 25 API, java.util.ArrayList
    locator: class description, fail-fast iterators
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/ConcurrentModificationException.html
    title: Java SE 25 API, java.util.ConcurrentModificationException
    locator: class description
  - url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/Iterator.html
    title: Java SE 25 API, java.util.Iterator
    locator: interface description and method summary
  - url: https://docs.oracle.com/javase/specs/jls/se25/html/jls-14.html#jls-14.14.2
    title: The Java Language Specification, Java SE 25 Edition
    locator: 14.14.2 The enhanced for statement
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/language-specification/statements#1395-the-foreach-statement
    title: C# language specification, Statements
    locator: 13.9.5 The foreach statement; 13.9.5.2 Synchronous foreach
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1.getenumerator
    title: .NET API, List<T>.GetEnumerator Method
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerator-1
    title: .NET API, IEnumerator<T> Interface
    locator: Remarks
  - url: https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/statements/yield#execution-of-an-iterator
    title: C# language reference, yield statement
    locator: Execution of an iterator
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
  - url: https://go.dev/doc/go1.23#language
    title: Go 1.23 Release Notes
    locator: Changes to the language
  - url: https://pkg.go.dev/iter#hdr-Iterators
    title: Go standard library, package iter
    locator: Iterators; Single-Use Iterators
  - url: https://pkg.go.dev/iter#hdr-Pulling_Values
    title: Go standard library, package iter
    locator: Pulling Values; func Pull
  - url: https://doc.rust-lang.org/std/iter/index.html#iterator
    title: The Rust Standard Library, Module std::iter
    locator: Iterator
  - url: https://doc.rust-lang.org/std/iter/index.html#laziness
    title: The Rust Standard Library, Module std::iter
    locator: Laziness
  - url: https://doc.rust-lang.org/std/iter/index.html#infinity
    title: The Rust Standard Library, Module std::iter
    locator: Infinity
  - url: https://doc.rust-lang.org/book/ch13-04-performance.html
    title: The Rust Programming Language
    locator: 13.4 Comparing Performance, Loops vs. Iterators
  - url: https://doc.rust-lang.org/reference/expressions/loop-expr.html#iterator-loops
    title: The Rust Reference, Loops and other breakable expressions
    locator: Iterator loops, rules expr.loop.for.intro and expr.loop.for.desugar
  - url: https://timsong-cpp.github.io/cppwp/n4950/stmt.ranged
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[stmt.ranged], paragraph 1"
  - url: https://timsong-cpp.github.io/cppwp/n4950/vector.modifiers
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[vector.modifiers], paragraphs 2 and 3"
  - url: https://timsong-cpp.github.io/cppwp/n4950/iterator.requirements.general
    title: Working Draft, Programming Languages, C++ (N4950, the C++23 draft)
    locator: "[iterator.requirements.general], paragraphs 1 and 2"
  - url: https://kotlinlang.org/docs/iterators.html
    title: Kotlin documentation, Iterators
    locator: Iterators
  - url: https://kotlinlang.org/docs/sequences.html
    title: Kotlin documentation, Sequences
    locator: introduction, Iterable and Sequence processing
  - url: https://kotlinlang.org/docs/control-flow.html#iterators
    title: Kotlin documentation, Conditions and loops
    locator: Iterators
  - url: https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_pipelines#one-at-a-time-processing
    title: PowerShell documentation, about_Pipelines
    locator: One-at-a-time processing
  - url: https://www.gnu.org/software/bash/manual/bash.html#Looping-Constructs
    title: Bash Reference Manual
    locator: 3.2.5.1 Looping Constructs, for
  - paper: doi:10.1145/359763.359789
    title: Barbara Liskov, Alan Snyder, Russell Atkinson and Craig Schaffert, Abstraction mechanisms in CLU, Communications of the ACM 20(8), 1977
    locator: iterators and the for statement
  - paper: doi:10.1145/359763.359782
    title: Mary Shaw, William A. Wulf and Ralph L. London, Abstraction and verification in Alphard, defining and specifying iteration and generators, Communications of the ACM 20(8), 1977
    locator: generators and the for statement of Alphard
  - paper: doi:10.1145/357133.357136
    title: Ralph E. Griswold, David R. Hanson and John T. Korb, Generators in Icon, ACM Transactions on Programming Languages and Systems 3(2), 1981
    locator: the paper as a whole
  - paper: doi:10.1145/154766.155367
    title: Barbara Liskov, A History of CLU, HOPL-II, 1993
    locator: 3.10 Iterators
  - url: http://stepanovpapers.com/STL/DOC.PDF
    title: Alexander Stepanov and Meng Lee, The Standard Template Library, Hewlett-Packard Laboratories, October 31, 1995
    locator: 5 Iterators
  - url: https://go.dev/doc/devel/release#go1.23.0
    title: Go Release History
    locator: go1.23.0
---

## Idea

A program often visits the elements of a collection, one after the other. The collection can be a list, the lines of a file, the keys of a table or the nodes of a tree. Each kind of collection stores its elements in a different way, so each needs different code to find the next element. An iterator hides that code. It is an object that gives the elements one at a time and remembers where it is between calls. The code that uses the iterator knows only two operations: get the next element, and find out that no elements are left.[^4]

With iterators, one loop statement works for every kind of collection. The `for` loop of Python, the `for...of` loop of JavaScript, the enhanced `for` of Java, `foreach` of C# and the `for` loop of Rust all take an iterable value and drive its iterator. Library functions such as `sum`, `max` or `collect` also take any iterable, so they do not need one version for each collection.

In Python, `iter()` asks a list for an iterator, and `next()` asks the iterator for one element. When no elements are left, `next()` raises `StopIteration`.[^2]

```python run id=idea
colors = ["red", "green"]
it = iter(colors)
print(next(it))
print(next(it))
try:
    next(it)
except StopIteration:
    print("no more elements")
```

```output for=idea
red
green
no more elements
```

The list does not change. The iterator holds the position, so two iterators over the same list move independently.

## How it works

An iteration protocol has three parts. The iterable is the value that can be visited, such as a list. The iterator is a separate object that holds the state of one pass: a position, and often a reference to the iterable. The protocol is the set of methods that the loop statement calls. A loop statement first asks the iterable for an iterator, once. Then it asks the iterator for elements until the iterator signals the end.[^5]

Each language names these operations in its own way. The table shows the names.

| Language | Get an iterator | Get the next element | End of the elements |
|---|---|---|---|
| Python | `iter(x)`, which calls `x.__iter__()` | `next(it)`, which calls `it.__next__()` | `__next__` raises `StopIteration`[^2] |
| JavaScript, TypeScript | `x[Symbol.iterator]()` | `it.next()` returns `{ value, done }` | `done` is `true`[^7][^8] |
| Java | `x.iterator()` | `it.hasNext()`, then `it.next()` | `hasNext()` returns `false`[^15] |
| Kotlin | `x.iterator()` | `it.hasNext()`, then `it.next()` | `hasNext()` returns `false`[^35] |
| C#, PowerShell | `x.GetEnumerator()` | `e.MoveNext()`, then `e.Current` | `MoveNext()` returns `false`[^18] |
| Rust | `x.into_iter()` | `it.next()` returns `Option<T>` | `next()` returns `None`[^25] |
| C++ | `begin(x)` and `end(x)` | `*it` reads, `++it` moves | `it == end`[^30] |
| Go | the value is a function | the function calls `yield(v)` | the function returns[^21] |

A loop statement is a short form of a `while` loop over these calls. The Java specification gives the enhanced `for` statement as a basic `for` loop that calls `iterator()`, `hasNext()` and `next()`.[^16] The C++ standard defines the range-based `for` statement as a loop from `begin` to `end` with `++`.[^30] The Rust Reference defines a `for` loop as a call of `IntoIterator::into_iter` and a `loop` that calls `next()` until `None`.[^29] In C#, `foreach` uses any type that has a suitable `GetEnumerator` method, `MoveNext` method and `Current` property, even when the type does not implement `IEnumerable`.[^17] The Python `for` statement written as a `while` loop looks like this:

```python run id=desugar
colors = ["red", "green"]
it = iter(colors)
while True:
    try:
        color = next(it)
    except StopIteration:
        break
    print(color)
```

```output for=desugar
red
green
```

The iterator and the iterable can be one object. In Python, the `__iter__` method of an iterator returns the iterator itself, so an iterator can go where an iterable is expected.[^1] A list is a container, not an iterator: it makes a fresh iterator for each call of `iter()`. An iterator has only one pass. After it is exhausted, it keeps signalling the end, so a second loop over it sees no elements.[^4]

The end rule is not the same everywhere. Python says that an iterator that has raised `StopIteration` must raise it on each later call, and an iterator that does not is broken.[^2] ECMAScript says that a `next` call after `done: true` should also return `done: true`, but the language does not enforce this.[^8] Rust allows an iterator to return `Some` again after `None`, and `TryIter` is one that does.[^25] Code that needs the end to stay the end must not assume it.

An iterator can also be told that the loop stopped early. In JavaScript, an iterator can have an optional `return` method. When a `for...of` loop ends through `break`, `return` or an exception, the language calls it, so the iterator can close a file or free other resources.[^9][^8] The JavaScript example shows this. In C#, `foreach` is defined as a `while` loop inside `try` and `finally`, and the `finally` block disposes of the enumerator.[^17]

## In languages

The example files show the same countdown iterator in each language. The table gives the verdict for each tier 1 language.

| Language | Verdict | Form |
|---|---|---|
| Python | native | `__iter__` and `__next__`; generator functions |
| JavaScript | native | `Symbol.iterator` and `next()`; generator functions; iterator helpers |
| TypeScript | native | as JavaScript, with the types `Iterable<T>` and `Iterator<T>` |
| Java | native | `Iterable<T>` and `Iterator<T>`; enhanced `for` |
| C | idiom | a struct that holds the position and a `next` function |
| C++ | native | iterator types with `begin` and `end`; range-based `for` |
| C# | native | `IEnumerable<T>` and `IEnumerator<T>`; `foreach`; `yield return` |
| Go | native | functions of type `iter.Seq` and `iter.Seq2`; `range` over a function |
| Rust | native | the trait `Iterator`; `for` over `IntoIterator` |
| Kotlin | native | `Iterable<T>` and `Iterator<T>`; `Sequence<T>` for lazy chains |
| PowerShell | native | .NET enumerators; the pipeline enumerates its input |
| Bash | idiom | `for` over an expanded word list; `while read` over a stream |
| SQL | n/a | a query describes a result set, not a loop |
| Regex | n/a | a pattern has no loops over values |
| HTML/CSS | n/a | markup and style rules have no loops |

C has no iterator protocol and no loop statement that calls one. A pointer is the iterator of an array. For other data, the usual form is a struct that holds the position and a function that returns the next element, as the C example shows. Nothing in the language connects the two, and nothing checks that the data stays valid.

Bash has no iterator objects. The `for` loop expands its words first and then runs the body once for each word in the resulting list.[^37] To process a stream item by item, a `while read` loop reads one line at a time from a pipe, as the Bash example shows.

## Variations and related ideas

- **External and internal iterators.** With an external iterator, the caller asks for each element. The iterators of Python, Java, C#, Rust and C++ work this way. With an internal iterator, the iterator calls the loop body for each element. Go iterator functions work this way: the loop body becomes the `yield` function, and `yield` returns `false` when the loop wants to stop.[^23] The Go documentation calls these push iterators, and `iter.Pull` turns one into a pull iterator with a `next` function and a `stop` function.[^24] An external iterator can walk two sequences side by side. An internal iterator cannot, which was a limit of the iterators of CLU.[^41]
- **Generators.** Writing an iterator class by hand means storing the position in fields. A generator function stores it in a suspended call: the body runs up to a `yield`, gives a value, and continues from there on the next request. In C#, a call of a method with `yield return` does not run the body. The body runs up to the first `yield return` when the caller asks for the first element.[^20] The C# example shows this order.
- **Lazy chains.** Adapters such as `map`, `filter` and `take` can wrap an iterator in another iterator. No element is computed until a consumer asks for one. Rust adapters are lazy,[^26] JavaScript iterator helpers are lazy transformations of a source iterator,[^10] and Kotlin sequences process each element through the whole chain before the next element starts.[^34]
- **Infinite iterators.** An iterator need not end. The Rust range `0..` never returns `None`, so a program must limit it with an adapter such as `take`. A consumer that reads to the end, such as `min`, never returns on an infinite iterator.[^27]
- **Iterator categories in C++.** A C++ iterator is a position that generalizes a pointer. The standard defines six categories: input, output, forward, bidirectional, random access and contiguous. An input iterator allows one pass, and a forward iterator allows several.[^32]
- **Cursors.** A database cursor gives the rows of a result set one at a time and plays the role of an iterator for a query. The SQL language itself describes the result, not the loop.
- **Recursion.** A recursive function visits a tree with the call stack as its state. An iterator over the same tree must keep an explicit stack of the nodes that are left. A generator can use recursion inside and still give one node at a time. See [[recursion]].
- **Async iterators.** When each element comes from input and output, the call that gets the next element can return a future or a promise, and the loop awaits it. See [[async-await]].

## Pitfalls

### Removing items while you loop skips items

A list iterator holds an index. When the loop removes the element at that index, the elements after it move one place back. The iterator then moves forward and skips the element that took the old place. The Python tutorial says that code that changes a collection while it iterates over it can be tricky.[^6]

```python wrong id=remove-skip
numbers = [1, 2, 2, 3]
for n in numbers:
    if n == 2:
        numbers.remove(n)
print(numbers)
```

```output for=remove-skip
[1, 2, 3]
```

The fix is to make a fresh list, or to loop over a copy.[^6]

```python run id=remove-skip-fix
numbers = [1, 2, 2, 3]
numbers = [n for n in numbers if n != 2]
print(numbers)
```

```output for=remove-skip-fix
[1, 3]
```

A dictionary detects some changes. A loop over a dictionary that adds or deletes keys can raise `RuntimeError`, or it can fail to visit all entries.[^3] Loop over `list(ages)`, a copy of the keys, to delete safely.

```python fails expect=exception:RuntimeError id=dict-size
ages = {"ana": 31, "bo": 17}
for name in ages:
    if ages[name] < 18:
        del ages[name]
```

### A fail-fast iterator does not catch every change

The iterators of `java.util.ArrayList` are fail-fast. When the list changes in structure during the loop, by any means other than the iterator's own `remove` or `add`, the next call of `next()` throws `ConcurrentModificationException`.[^13][^14]

```java fails expect=exception:ConcurrentModificationException id=cme
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> numbers = new ArrayList<>(List.of(1, 2, 3, 4));
        for (Integer n : numbers) {
            if (n == 2) {
                numbers.remove(n);
            }
        }
        System.out.println(numbers);
    }
}
```

The check is not guaranteed. The documentation says that it is wrong to write a program that depends on this exception.[^13] When the loop removes the second-to-last element, `hasNext()` returns `false` at once. The loop ends with no exception, and it never visits the last element.

```java wrong id=cme-silent
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> numbers = new ArrayList<>(List.of(1, 2, 3));
        List<Integer> seen = new ArrayList<>();
        for (Integer n : numbers) {
            seen.add(n);
            if (n == 2) {
                numbers.remove(n);
            }
        }
        System.out.println("seen " + seen + ", left " + numbers);
    }
}
```

```output for=cme-silent
seen [1, 2], left [1, 3]
```

The fix is to remove through the iterator, or to call `removeIf`.

```java run id=cme-fix
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<Integer> numbers = new ArrayList<>(List.of(1, 2, 3, 4));
        Iterator<Integer> it = numbers.iterator();
        while (it.hasNext()) {
            if (it.next() == 2) {
                it.remove();
            }
        }
        System.out.println(numbers);
    }
}
```

```output for=cme-fix
[1, 3, 4]
```

In C#, a change to a `List<T>` invalidates its enumerators, and the next `MoveNext` throws `InvalidOperationException`.[^18]

```csharp fails expect=exception:InvalidOperationException id=invalid-op
using System;
using System.Collections.Generic;

var numbers = new List<int> { 1, 2, 3 };
foreach (int n in numbers)
{
    if (n == 1)
    {
        numbers.Add(4);
    }
}
Console.WriteLine(numbers.Count);
```

### An invalid iterator in C++ is undefined behaviour

A C++ iterator of a `std::vector` points into the storage of the vector. When `push_back` must grow the storage, it moves the elements, and all iterators of the vector become invalid. When it does not grow, the iterators at and after the insertion point become invalid. `erase` invalidates the iterators at and after the erased element.[^31] The range-based `for` loop keeps its iterators across the body,[^30] so a `push_back` inside the loop can leave the loop reading freed memory. The program compiles, and AddressSanitizer reports the error at run time.

```cpp fails expect=sanitizer:address id=invalidate
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};
    for (int n : v) {
        if (n == 1) {
            v.push_back(4);
        }
        std::cout << n << '\n';
    }
}
```

Rust rejects the same program at compile time. The loop borrows `v`, and `push` needs a second, mutable borrow while the first is still alive.

```rust fails expect=error:E0502 id=borrow
fn main() {
    let mut v = vec![1, 2, 3];
    for n in &v {
        if *n == 1 {
            v.push(4);
        }
    }
    println!("{v:?}");
}
```

The fix in both languages is to collect the changes in a second container and apply them after the loop, or to loop over indexes and read the length again on each pass.

### An exhausted iterator looks empty

An iterator has one pass. A second loop over the same iterator sees no elements, and no error tells you so. The Python glossary says that an iterator used for a second pass looks like an empty container.[^4] Some functions return iterators, not lists. In Python, `map` is one of them, as the example shows. In Go, some iterator functions read from a stream and can walk it only once.[^23]

```python wrong id=exhausted
squares = map(lambda n: n * n, [1, 2, 3])
print(sum(squares))
print(max(squares, default=None))
```

```output for=exhausted
14
None
```

The fix is to store the values in a list when you need more than one pass.

```python run id=exhausted-fix
squares = list(map(lambda n: n * n, [1, 2, 3]))
print(sum(squares))
print(max(squares, default=None))
```

```output for=exhausted-fix
14
9
```

### A lazy adapter does nothing until something consumes it

An adapter only builds an iterator. If nothing pulls values from that iterator, the closure in it never runs. The Rust documentation shows this mistake: a `map` used for its side effects prints nothing, and the compiler warns that iterators are lazy and do nothing unless consumed.[^26]

```rust wrong id=lazy
fn main() {
    let v = vec![1, 2, 3];
    v.iter().map(|n| println!("{n}"));
    println!("done");
}
```

```output for=lazy
done
```

Use a `for` loop for side effects. Use `map` to compute values that a consumer such as `collect` or `sum` reads.

### Map order is not a stable order

The Go specification does not say in which order a `range` loop visits the entries of a map. It also says that the order is not guaranteed to be the same from one loop to the next.[^21] Code that prints a map in a loop, or that compares two results, gives different output on different runs. Sort the keys when the order matters.

```go run id=sorted-keys
package main

import (
	"fmt"
	"maps"
	"slices"
)

func main() {
	ages := map[string]int{"cy": 25, "ana": 31, "bo": 17}
	for _, name := range slices.Sorted(maps.Keys(ages)) {
		fmt.Println(name, ages[name])
	}
}
```

```output for=sorted-keys
ana 31
bo 17
cy 25
```

`maps.Keys` returns an iterator, and `slices.Sorted` reads it to the end and returns a sorted slice.

## Cost

- **One call for each element.** A loop over an iterator makes at least one call for each element: `__next__` in Python, `hasNext` and `next` in Java and Kotlin, `MoveNext` and `Current` in C#. The Java enhanced `for` statement makes these calls through the `Iterator` interface.[^16]
- **Inlined adapters.** In Rust and C++, each iterator and each adapter is a distinct type, so the compiler knows which `next` or `++` runs and can inline it. The Rust book calls iterators a zero-cost abstraction: a chain of adapters compiles to roughly the same code as a hand-written loop.[^28] The C++ standard defines iterators as a generalization of pointers, and a contiguous iterator works like a pointer into an array.[^32]
- **Laziness.** A lazy chain holds one element at a time and builds no intermediate collection. An eager chain, such as Kotlin operations on an `Iterable`, makes a full list after each step. The Kotlin documentation also says that the laziness of sequences has its own overhead, which can be significant for small collections or small computations.[^34]
- **Loops with no iterator object.** The Kotlin compiler turns a `for` loop over a range or an array into an index loop.[^35] A Python `for` loop over a list still makes an iterator object for the list.[^5]
- **Boxing.** The Java type `Iterator<Integer>` returns objects, not `int` values, so a loop over a `List<Integer>` reads one boxed object for each element.[^15]

## History

- **1975 and 1977.** The CLU group learned of the generators of Alphard during a visit to Carnegie Mellon University in the summer of 1975. Russ Atkinson then designed the CLU iterator, a single procedure that yields its results one at a time to the body of a `for` statement.[^41] An Alphard generator was a group of operations, while a CLU iterator was one operation. Both designs were published in 1977.[^38][^39]
- **1981.** Ralph Griswold, David Hanson and John Korb described the generators of the Icon language.[^40]
- **1995.** Alexander Stepanov and Meng Lee described the Standard Template Library for C++. Its iterators are a generalization of pointers, and the report defines five categories of them.[^42] The C++ standard later added a sixth category, contiguous iterators.[^32]
- **2001.** PEP 234 by Ka-Ping Yee and Guido van Rossum defined the iterator protocol of Python: the built-in `iter()`, the `__iter__` method, an iterator method called `next()` and the `StopIteration` exception.[^1] In the Python 3.14 documentation, that method is `__next__`.[^2]
- **2015.** ECMAScript 2015 added the iteration protocols, the iterable interface and the iterator interface, to JavaScript.[^11]
- **2024.** Go 1.23, released on 13 August 2024, let `range` loop over iterator functions.[^22][^43]
- **2025.** ECMAScript 2025 added iterator helpers such as `Iterator.prototype.map`.[^12]

## References
