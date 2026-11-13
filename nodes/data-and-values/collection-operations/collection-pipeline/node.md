---
tag: B105KH
kind: pattern
slug: collection-pipeline
name: Collection pipeline
aliases: [pipeline of collection operations, method chain, stream pipeline, LINQ method chain, iterator chain, map filter reduce, chain of operations]
area: data-and-values/collection-operations
level: core
summary: >-
  Write a computation over a collection as a sequence of operations such as filter, map, sort and reduce. Each operation takes a collection from the step before and gives a collection to the next step.
family: architectural
polarity: positive
intent: "Organize a computation over a collection as a sequence of small operations that each take a collection and give a new collection to the next operation."
also_known_as: [Pipes and Filters (the general form)]
history:
  year: 2014
  people: [Martin Fowler]
  note: Fowler gave the name Collection Pipeline to the pattern in an article that he published in parts from July 2014 and last revised in June 2015. He first saw the pattern in Unix pipelines and in Smalltalk collection methods.
see_also: [replace-loop-with-pipeline, comprehension, map-over-a-list, filter-a-list, reduce-a-list-to-a-value, iterators, laziness, pipeline-concurrency]
sources:
- url: https://martinfowler.com/articles/collection-pipeline/
  title: Martin Fowler, Collection Pipeline
  locator: Introduction; Defining Collection Pipeline; Alternatives; Laziness; Parallelism; Immutability; Debugging; When to Use It; Operation Catalog
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.base/java/util/stream/package-summary.html
  title: Package java.util.stream, Java SE 25 API
  locator: Stream operations and pipelines; Parallelism; Side-effects
- url: https://kotlinlang.org/docs/sequences.html
  title: Sequences, Kotlin documentation
  locator: Introduction
- url: https://hexdocs.pm/elixir/Stream.html
  title: Stream, Elixir documentation
  locator: Introduction
- url: https://docs.ruby-lang.org/en/master/Enumerator/Lazy.html
  title: class Enumerator::Lazy, Ruby documentation
  locator: Introduction
- url: https://learn.microsoft.com/en-us/dotnet/standard/linq/deferred-execution-lazy-evaluation
  title: Deferred execution and lazy evaluation, Microsoft Learn
  locator: Deferred execution; Eager and lazy evaluation
- url: https://lodash.com/docs/4.17.15
  title: Lodash Documentation
  locator: _(value); _.chain
- url: https://doc.rust-lang.org/std/iter/index.html
  title: Module iter, The Rust Standard Library
  locator: Adapters; Laziness
- url: https://pkg.go.dev/iter
  title: Package iter, Go standard library
  locator: Iterators; type Seq
- url: https://refactoring.com/catalog/replaceLoopWithPipeline.html
  title: Martin Fowler, Catalog of Refactorings, Replace Loop with Pipeline
  locator: the entry
- url: https://learn.microsoft.com/en-us/dotnet/csharp/linq/standard-query-operators/
  title: Standard query operators overview, Microsoft Learn
  locator: Introduction
- url: https://docs.python.org/3/library/itertools.html
  title: itertools, Python documentation
  locator: Introduction
- url: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Iterator
  title: Iterator, MDN Web Docs
  locator: Iterator helper methods
- url: https://en.cppreference.com/w/cpp/ranges.html
  title: Ranges library, cppreference.com
  locator: Introduction
---

## Intent

Write a computation over a collection as a sequence of small, named operations. Fowler defines the pattern: "A collection pipeline lays out a sequence of operations that pass a collection of items between them. Each operation takes a collection as an input and emits another collection (except the last operation, which may be a terminal that emits a single value)"[^1]. The common operations are filter, map, sort, flat map, group by and reduce.

The pattern is the reason that most languages now have methods such as `filter` and `map` on their collections or streams. This page explains the pattern, the difference between eager and lazy pipelines, and when a loop is the better choice.

## Problem

A blog has articles. Each article has a title, a number of words and a list of tags. A program must find the titles of the two longest articles with the tag `nosql`, and the total number of words in all the articles with that tag. A first version uses one loop:

```python run id=one-loop
articles = [
    ("Intro to NoSQL", 1200, ["nosql", "databases"]),
    ("Event sourcing", 800, ["architecture"]),
    ("Graph stores", 1500, ["nosql", "graphs"]),
    ("Refactoring loops", 600, ["refactoring"]),
    ("Key-value stores", 900, ["nosql"]),
]

# One loop does three jobs: it selects, it sums and it collects.
selected = []
total = 0
for title, words, tags in articles:
    if "nosql" in tags:
        selected.append((words, title))
        total += words
selected.sort(reverse=True)
top = []
for words, title in selected:
    if len(top) == 2:
        break
    top.append(title)
print(top, total)
```

```output for=one-loop
['Graph stores', 'Intro to NoSQL'] 3600
```

The result is correct, but the reader must find the parts of the computation in the body of the loop. The `if` is the selection. The `append` and the `+=` are two different results. The second loop with `break` is a limit. Each new question about the articles adds a variable and more lines to the loop, and the jobs mix more. See [[loops-smell]].

## Solution

Give each job its own operation and connect the operations. Each operation gets a collection and gives a new collection. The last operation can give one value.

```python run id=named-steps
from functools import reduce

articles = [
    ("Intro to NoSQL", 1200, ["nosql", "databases"]),
    ("Event sourcing", 800, ["architecture"]),
    ("Graph stores", 1500, ["nosql", "graphs"]),
    ("Refactoring loops", 600, ["refactoring"]),
    ("Key-value stores", 900, ["nosql"]),
]


def words(article):
    return article[1]


def title(article):
    return article[0]


# filter: keep the NoSQL articles.
nosql = list(filter(lambda a: "nosql" in a[2], articles))
# sort, slice, map: the titles of the two longest ones.
top = list(map(title, sorted(nosql, key=words, reverse=True)[:2]))
# map, reduce: the total number of words.
total = reduce(lambda acc, n: acc + n, map(words, nosql), 0)
print(top, total)
```

```output for=named-steps
['Graph stores', 'Intro to NoSQL'] 3600
```

Each line now says one thing: keep these articles, take the two longest titles, add up the words. A new question is a new pipeline, not a change to a shared loop.

The parts of the pattern:

- **The source.** A list, an array, a set, the lines of a file, the rows of a query or a generator of values.
- **The operations.** Each one is small and does one thing. Most operations take a function as a parameter: the test for `filter`, the change for `map`, the key for `sort` and `group by`. See [[higher-order-functions]].
- **The composition.** Fowler lists three forms[^1]. A Unix shell connects processes with `|`. An object-oriented language chains methods, so `articles.filter(...).map(...)` reads from left to right. A functional language nests function calls, `map(f, filter(p, xs))`, which reads from the inside out, or uses an operator such as the Clojure `->>` macro to make the nested calls into a line.
- **The terminal.** The last step makes a list, a set, a map, a number or an effect such as printing. Java calls it the terminal operation: "A stream pipeline consists of a source (such as a Collection, an array, a generator function, or an I/O channel); followed by zero or more intermediate operations such as Stream.filter or Stream.map; and a terminal operation such as Stream.forEach or Stream.reduce"[^2].

The common operations have different names in different languages. Fowler's article has a catalog of them[^1]:

| Operation | Does | Other names |
|---|---|---|
| map | applies a function to each item | `collect` (Smalltalk, Ruby), `Select` (C#), `transform` (C++) |
| filter | keeps the items that pass a test | `select` (Ruby), `Where` (C#), `grep` (shell) |
| reject | drops the items that pass a test | `filterNot` (Kotlin, Scala), `grep -v` (shell) |
| flat map | maps each item to a collection and joins the results | `mapcat` (Clojure), `SelectMany` (C#), `flat_map` (Rust) |
| reduce | combines the items into one value | `fold`, `inject` (Smalltalk, Ruby), `Aggregate` (C#) |
| group by | puts the items into groups by a key | `groupingBy` (Java), `GroupBy` (C#), `Object.groupBy` (JavaScript) |
| sort | gives the items in order by a comparator or a key | `sorted` (Java, Python), `OrderBy` (C#) |
| take, drop | keeps or drops the first n items | `limit` and `skip` (Java), `head` and `tail` (shell) |
| distinct | drops repeated items | `uniq` (shell, Ruby), `Distinct` (C#) |

**Eager and lazy pipelines.** An eager pipeline makes a full collection after each step. A lazy pipeline sends the items through all the steps one at a time, and does no work until the terminal step asks for items. Java says of its intermediate operations: "They are always lazy; executing an intermediate operation such as filter() does not actually perform any filtering"[^2]. Kotlin gives the difference: for a collection, "each processing step completes and returns its result – an intermediate collection", but "multistep processing of sequences is executed lazily when possible: actual computing happens only when the result of the whole processing chain is requested"[^3]. A lazy pipeline can stop early. When the last step needs only the first match, the earlier steps run only for the items before it. Each example file counts the calls of a slow `score` function in the two forms, and gives 5 calls for the eager form and 1 call for the lazy form, except C++.

## Example

Each example file runs the same pipelines on the same five articles: the two longest NoSQL titles, the total words, the number of articles for each tag, and the eager and lazy forms of "the first score over 1000".

| Language | Operations | Lazy form |
|---|---|---|
| Python | `filter`, `map`, `sorted`, `functools.reduce`, `collections.Counter`, `itertools.chain` | generator expressions and `itertools.islice` |
| JavaScript | array methods `filter`, `map`, `toSorted`, `flatMap`, `reduce`, `Object.groupBy` | iterator helpers on `values()` |
| Java | `Stream` methods and `Collectors.groupingBy` | every stream; `toList` in the middle makes it eager |
| C# | LINQ `Where`, `Select`, `OrderByDescending`, `SelectMany`, `GroupBy`, `Aggregate` | every LINQ chain; `ToList` in the middle makes it eager |
| Go | the `slices` package and small `Map`, `Filter` and `Take` functions over `iter.Seq` | the iterator functions |
| Rust | iterator adapters `filter`, `map`, `take`, `flat_map`, `fold` | every iterator chain; `collect` in the middle makes it eager |
| C++ | range algorithms `copy_if`, `sort`, `transform`, and `std::accumulate` | views `transform`, `filter`, `take`, `join` |
| C | loops over arrays and `qsort` | one loop that does all the steps and stops with `break` |

The C++ lazy form gives 4 calls, not 1. A filter view calls the transform before it once to test an item and once more to give the item. The loop also moves the filter to the next match before `take(1)` stops it. A lazy pipeline does not promise one call for each item, so a slow or impure function in a pipeline needs care.

## Consequences

**Benefits.**

- **Each step has a name.** The reader sees filter, sort and take, not an `if`, a `sort` call and a counter with `break`. Fowler says that pipelines "can clearly capture what otherwise requires long and gnarly loops, and can help make code more readable and thus cheaper and easier to enhance"[^1].
- **The steps are small and can be tested alone.** A step is a function. A new step goes between two others and does not change them.
- **No shared state between the steps.** Each step makes a new collection and does not change its input, so pipelines go well with immutable data. Fowler: "Collection-pipelines naturally lend themselves to immutable data structures"[^1]. See [[immutability]].
- **Laziness and parallelism come from the library.** A lazy library stops early and needs no full collections between the steps. Some libraries run the same pipeline on many cores. Java says that "The only difference between the serial and parallel versions of this example is the creation of the initial stream"[^2], which calls `parallelStream()` instead of `stream()`.

**Liabilities.**

- **Copies.** An eager pipeline makes a new collection at each step. Fowler notes that "Done naively this involves a lot of copying, which can lead to problems with large amounts of data. However, most of time, it's not a problem in practice"[^1], because the copies hold references, not the items.
- **Overhead of laziness.** A lazy pipeline creates objects for its steps and calls them for each item. Kotlin says: "the lazy nature of sequences adds some overhead which may be significant when processing smaller collections or doing simpler computations"[^3].
- **Some steps need all the items.** A sort or a group by cannot give its first item before it has read the last one. Java: "Stateful operations may need to process the entire input before producing a result. For example, one cannot produce any results from sorting a stream until one has seen all elements of the stream"[^2]. Fowler gives the same rule: "Some pipeline operations cannot work with laziness and have to evaluate the whole list"[^1].
- **Side effects become hard to follow.** A lazy pipeline calls its functions when the terminal step runs, in an order that the library chooses. Java says that "Side-effects in behavioral parameters to stream operations are, in general, discouraged, as they can often lead to unwitting violations of the statelessness requirement, as well as other thread-safety hazards"[^2]. The `calls` counter in the examples is a side effect for the demonstration only.
- **Debugging.** A long chain is one expression, so it has no line for a breakpoint between two steps. Fowler suggests putting an intermediate result into a variable, or adding a print step in a `map`[^1].

## When not to use it

Do not use the pattern when the language makes each step heavy. Fowler tried to build pipelines in Java before it had lambdas, and wrote: "the problem is the that syntax is just too messy, overwhelming the clarity that makes collection pipelines so effective. So I gave up and used loops instead"[^1].

Do not build a long pipeline. Fowler's rule for functions applies to pipelines too: "Larger pipelines need to be factored into separate methods"[^1]. Give a group of steps a name, such as `index_by_tag`, and call it from a short pipeline.

Use a comprehension when the language has one and the job is one map and one filter. Fowler says that comprehensions "are often easier to work with for simple expressions, but you still need pipelines for their greater flexibility"[^1]. See [[comprehension]].

Use a loop when the steps share state that is not a collection, for example a running total that decides when to stop, or when a hot loop must avoid the calls and objects of the pipeline. Measure first.

## Variants and related patterns

**Pipes and Filters.** Fowler says that "Collection Pipeline is a special case of the Pipes and Filters pattern"[^1], the architectural pattern in which independent filters pass data through pipes. In a collection pipeline the data between the steps is a collection.

**Unix pipelines.** `grep`, `sort`, `uniq` and `head` connected with `|` are a collection pipeline whose items are lines of text. Fowler first saw the pattern there[^1].

**Method chains, nested calls and pipe operators.** Smalltalk, Ruby, JavaScript, Java, C#, Kotlin and Rust chain methods. Lisp and Python nest calls. Clojure `->>`, Elixir and F# `|>` and the C++ `|` on views write nested calls as a line.

**Lazy pipelines.** Java streams, .NET LINQ, Rust iterators, JavaScript iterator helpers, Kotlin sequences, Elixir streams, Ruby `Enumerator::Lazy`, Python iterators and C++ views are lazy. Elixir says: "Streams are composable, lazy enumerables"[^4]. Ruby says that `Enumerator::Lazy` "allows constructing chains of operations without evaluating them immediately, and evaluating values on as-needed basis"[^5]. The .NET documentation says: "Deferred execution means that the evaluation of an expression is delayed until its realized value is actually required"[^6]. Lodash adds laziness to array code: "The execution of chained methods is lazy, that is, it's deferred until _#value is implicitly or explicitly called"[^7], and it can merge steps by "shortcut fusion", which "avoids the creation of intermediate arrays"[^7].

**Iterator adapters.** Rust calls the lazy steps adapters: "Functions which take an Iterator and return another Iterator are often called ‘iterator adapters’, as they’re a form of the ‘adapter pattern’"[^8]. Go has no adapters in its standard library, but its iterators are functions that call `yield`, so a program can write `Map`, `Filter` and `Take` as the Go example does. The Go documentation says: "An iterator is a function that passes successive elements of a sequence to a callback function, conventionally named yield"[^9]. See [[iterators]] and [[iterator-pattern]].

**Comprehensions.** A comprehension writes a map, a filter and a flat map as one expression. Fowler says that comprehensions "mirror simple collection pipelines"[^1]. See [[comprehension]].

**Replace Loop with Pipeline.** The refactoring that turns a loop into a collection pipeline, one step at a time[^10]. See [[replace-loop-with-pipeline]].

**Concurrent pipelines.** When the steps run in different threads or processes and pass items through queues or channels, the pipeline is a concurrency pattern. See [[pipeline-concurrency]].

## Known uses

- Java: the package `java.util.stream` is a library for collection pipelines. Its documentation describes a stream pipeline as a source, intermediate operations and a terminal operation, and it can run the same pipeline in parallel[^2].
- .NET: the LINQ standard query operators, such as `Where`, `Select`, `GroupBy` and `OrderBy`. The documentation says: "The standard query operators are the keywords and methods that form the LINQ pattern"[^11].
- Python: the `itertools` module gives lazy steps for pipelines. It says that its tools "form an “iterator algebra” making it possible to construct specialized tools succinctly and efficiently in pure Python"[^12].
- JavaScript: array methods for eager pipelines, and the iterator helper methods for lazy pipelines[^13]. Lodash gives chains with `_.chain`[^7].
- Kotlin: the collection functions for eager pipelines and `Sequence` for lazy ones. "Sequences offer the same functions as Iterable but implement another approach to multistep collection processing"[^3].
- C++: the ranges library. "Adaptors can be composed into pipelines, so that their actions take place as the view is iterated"[^14].

## References
