---
tag: BP6EDV
kind: concept
slug: map-filter-reduce
name: Map, filter and reduce
aliases: [map, filter, reduce, fold, map-reduce, apply-to-all, accumulate, aggregate, select and where]
area: functions-and-abstraction/functions-as-values
level: intro
summary: >-
  Map, filter and reduce are three higher-order functions on collections. Map changes each element, filter keeps the elements that pass a test, and reduce combines all elements into one value.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1959
  note: Lisp had a map function called maplist in 1959, with other versions in 1958. The MapReduce system of Google, described in 2004, took its name from the map and reduce of Lisp and other functional languages.
see_also: [higher-order-functions, first-class-function, anonymous-function, map-over-a-list, reduce-a-list-to-a-value, collection-pipeline, replace-loop-with-pipeline, function-composition]
sources:
- url: https://en.wikipedia.org/wiki/Map_(higher-order_function)
  title: Map (higher-order function), Wikipedia
  locator: the introduction; Optimizations; Language comparison
- url: https://en.wikipedia.org/wiki/Filter_(higher-order_function)
  title: Filter (higher-order function), Wikipedia
  locator: the introduction; Example
- url: https://en.wikipedia.org/wiki/Fold_(higher-order_function)
  title: Fold (higher-order function), Wikipedia
  locator: the introduction; On lists; Evaluation order considerations
- url: https://docs.python.org/3/library/functions.html#map
  title: Built-in Functions, The Python Standard Library
  locator: map; filter
- url: https://docs.python.org/3/library/functools.html#functools.reduce
  title: functools, The Python Standard Library
  locator: functools.reduce
- url: https://docs.python.org/3/whatsnew/3.0.html
  title: What's New In Python 3.0
  locator: Builtins
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map
  title: Array.prototype.map(), MDN Web Docs
  locator: the introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/filter
  title: Array.prototype.filter(), MDN Web Docs
  locator: the introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/reduce
  title: Array.prototype.reduce(), MDN Web Docs
  locator: the introduction; Parameters; Exceptions
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
  title: Package java.util.stream, Java SE 25 API
  locator: the package description; Stream operations and pipelines; Reduction operations
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/standard-query-operators/
  title: Standard query operators overview, C# documentation
  locator: Query expression syntax for standard query operators
- url: https://learn.microsoft.com/en-us/dotnet/api/system.linq.enumerable.aggregate
  title: Enumerable.Aggregate Method, .NET API
  locator: Overloads
- url: https://en.cppreference.com/w/cpp/algorithm/transform.html
  title: std::transform, C++ reference, cppreference.com
  locator: the description
- url: https://en.cppreference.com/w/cpp/algorithm/copy.html
  title: std::copy, std::copy_if, C++ reference, cppreference.com
  locator: the description
- url: https://en.cppreference.com/w/cpp/algorithm/accumulate.html
  title: std::accumulate, C++ reference, cppreference.com
  locator: the description
- url: https://doc.rust-lang.org/std/iter/trait.Iterator.html
  title: Trait Iterator, The Rust Standard Library
  locator: map; filter; fold
- url: https://go.dev/ref/spec#For_range
  title: The Go Programming Language Specification
  locator: For statements with range clause
- url: https://www.gnu.org/software/bash/manual/html_node/Looping-Constructs.html
  title: Looping Constructs, Bash Reference Manual
  locator: for
- url: https://www.usenix.org/legacy/events/osdi04/tech/full_papers/dean/dean_html/
  title: MapReduce, Simplified Data Processing on Large Clusters, Jeffrey Dean and Sanjay Ghemawat, OSDI 2004
  locator: 1 Introduction
- url: https://docs.python.org/3/glossary.html#term-iterator
  title: Glossary, The Python Documentation
  locator: iterator
---

## Idea

Map, filter and reduce are three functions that take a function and a collection. Each one replaces a common kind of loop.

- **Map** "applies a given function to each element of a collection, e.g. a list or set, returning the results in a collection of the same type."[^1]
- **Filter** produces a new collection "containing exactly those elements of the original data structure for which a given predicate returns the Boolean value true."[^2]
- **Reduce**, also called fold, combines the elements into one value. "Fold is also termed as reduce, accumulate, aggregate, compress, or inject."[^3]

```python run id=idea
from functools import reduce

nums = [1, 2, 3, 4, 5, 6]
print(list(map(lambda x: x * x, nums)))
print(list(filter(lambda x: x % 2 == 0, nums)))
print(reduce(lambda acc, x: acc + x, nums, 0))

evens = filter(lambda x: x % 2 == 0, nums)
squares = map(lambda x: x * x, evens)
print(reduce(lambda acc, x: acc + x, squares, 0))
```

```output for=idea
[1, 4, 9, 16, 25, 36]
[2, 4, 6]
21
56
```

The last three lines join the three functions. Filter keeps 2, 4 and 6, map squares them to 4, 16 and 36, and reduce adds them to 56.

## How it works

1. **Map.** The function takes one element and returns one new element. The result has the same number of elements as the input. In Python, `map` will "Return an iterator that applies function to every item of iterable, yielding the results."[^4]
2. **Filter.** The function is a predicate: it takes one element and returns true or false. Python `filter` will "Construct an iterator from those elements of iterable for which function is true."[^4]
3. **Reduce.** The function takes two values: the result so far, often called the accumulator, and the next element. Python `reduce` will "Apply function of two arguments cumulatively to the items of iterable, from left to right, so as to reduce the iterable to a single value."[^5] An initial value starts the accumulator and is the result for an empty collection.
4. **Left and right folds.** The order of the combination can change the result. "With a right fold, the sum would be parenthesized as 1 + (2 + (3 + (4 + 5))), whereas with a left fold it would be parenthesized as (((1 + 2) + 3) + 4) + 5."[^3] For an associative operation such as `+`, both give the same value[^3].

## In languages

- **Python.** The built-in functions `map` and `filter`, and `functools.reduce`[^4][^5]. Python 3 moved `reduce` out of the built-in functions. The release notes say: "Use functools.reduce() if you really need it; however, 99 percent of the time an explicit for loop is more readable."[^6]
- **JavaScript.** The array methods `map`, `filter` and `reduce`. `map` "creates a new array populated with the results of calling a provided function on every element in the calling array."[^7] `filter` "creates a shallow copy of a portion of a given array, filtered down to just the elements from the given array that pass the test implemented by the provided function."[^8] `reduce` runs "a user-supplied "reducer" callback function on each element of the array"[^9].
- **TypeScript.** The same array methods as JavaScript, with types for the callbacks.
- **Java.** Streams. The package has "functional-style operations on streams of elements, such as map-reduce transformations on collections."[^10] "A reduction operation (also called a fold) takes a sequence of input elements and combines them into a single summary result by repeated application of a combining operation"[^10].
- **C#.** LINQ. The method `Select` is the `select` clause and `Where` is the `where` clause of a query expression[^11]. `Aggregate` "Applies an accumulator function over a sequence."[^12]
- **C++.** The algorithms `std::transform`, `std::copy_if` and `std::accumulate`. `std::transform` "Applies the given function to the elements of the given source range(s), and stores the result in the destination range"[^13]. "copy_if only copies the elements for which the predicate pred returns true"[^14]. `std::accumulate` "Computes the sum of the given value init and all elements of the target range"[^15], or uses the operation `op` that the caller gives[^15].
- **Rust.** The iterator methods `map`, `filter` and `fold`. `map` "Takes a closure and creates an iterator which calls that closure on each element."[^16] `fold` "Folds every element into an accumulator by applying an operation, returning the final result."[^16]
- **Go.** A `for` loop with a `range` clause. It "iterates through all entries of an array, slice, string or map"[^17]. A program can write generic helper functions for map, filter and reduce.
- **C.** A `for` loop over an array. A helper function can take a function pointer for the operation. See [[function-pointer-and-delegate]].
- **Bash.** A `for` loop over words. The loop will "execute commands once for each word in the resultant list, with name bound to the current word."[^18]

## Variations and related ideas

- **Lazy or eager.** Python `map` and `filter` return iterators[^4]. In Java, "Intermediate operations are always lazy."[^10] Rust `map` is lazy too: "as map() is lazy, it is best used when you’re already working with other iterators."[^16] JavaScript array methods make a new array at once[^7].
- **Map fusion.** Two maps in a row can become one map of the composed function. "this type of optimization is known as map fusion and is the functional analog of loop fusion."[^1] See [[function-composition]].
- **Reduce without an initial value.** JavaScript `reduce` and Rust `reduce` can start with the first element. In JavaScript, "If initialValue is not specified, accumulator is initialized to the first value in the array"[^9]. Rust `reduce` returns an `Option`: "If the iterator is empty, returns None"[^16].
- **MapReduce.** Dean and Ghemawat named a system for large clusters after these functions. Its abstraction was inspired by the map and reduce of Lisp and "many other functional languages"[^19].
- **Pipelines.** A chain of map, filter and reduce calls is a pipeline. See [[collection-pipeline]] and [[replace-loop-with-pipeline]]. The task pages [[map-over-a-list]] and [[reduce-a-list-to-a-value]] show each step in all the languages.

## Pitfalls

### A Python map can be used only once

Python `map` and `filter` return iterators. "When no more data are available a StopIteration exception is raised instead. At this point, the iterator object is exhausted"[^20]. A second loop over the same object sees nothing:

```python wrong id=python-map-twice
squares = map(lambda x: x * x, [1, 2, 3])
print(sum(squares))
print(sum(squares))
```

```output for=python-map-twice
14
0
```

Call `list(...)` on the result when the program needs it more than once.

### Reduce on an empty collection

Without an initial value, a reduce has no result for an empty collection. JavaScript `reduce` throws a `TypeError`, "Thrown if the array contains no elements and initialValue is not provided."[^9]

```javascript wrong id=javascript-reduce-empty
try {
  const total = [].reduce((acc, x) => acc + x);
  console.log(total);
} catch (e) {
  console.log(e.name);
}
```

```output for=javascript-reduce-empty
TypeError
```

Give the initial value: `[].reduce((acc, x) => acc + x, 0)` returns 0.

### A lazy map that never runs

A lazy map does no work until a program reads its result. Rust says: "If you’re doing some sort of looping for a side effect, it’s considered more idiomatic to use for than map()."[^16] Use a `for` loop, or a method such as `for_each`, for side effects.

## Cost

Each step makes one pass over the data. An eager map or filter also makes a new collection. Wikipedia says that two maps in a row are slower "because each map requires rebuilding an entire list from scratch"[^1]. Lazy streams and iterators pass each element through all the steps and make no list in the middle[^10][^16]. A loop that does all the work in one pass has the same cost as a lazy pipeline.

## History

"The map function originated in functional programming languages."[^1] Lisp introduced a map function called `maplist` in 1959, "with slightly different versions already appearing in 1958."[^1] In 2004, Dean and Ghemawat described MapReduce, a library for computations on large clusters, and named it after the map and reduce of functional languages[^19].

## References
