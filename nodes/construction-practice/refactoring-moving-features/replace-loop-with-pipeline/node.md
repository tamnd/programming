---
tag: NPJT0B
kind: technique
slug: replace-loop-with-pipeline
name: Replace loop with pipeline
aliases: [replace loop with collection pipeline]
area: construction-practice/refactoring-moving-features
level: core
summary: >-
  Replace a loop that filters, transforms and collects items with a collection pipeline of operations such as filter, map and reduce; each language has its own form.
family: refactoring
polarity: positive
intent: "Show the steps of a computation over a collection as a sequence of named operations."
also_known_as: []
history:
  year: 2018
  people: [Martin Fowler]
  note: Replace Loop with Pipeline is in the chapter on moving features of the second edition of Refactoring (2018). Martin Fowler described the collection pipeline as a pattern in an article of 2015.
see_also: [loops-smell, split-loop, higher-order-functions, iterators, laziness]
sources:
  - url: https://martinfowler.com/articles/collection-pipeline/
    title: Martin Fowler, Collection Pipeline, 2015
    locator: Defining Collection Pipeline; the comparison with comprehensions
  - url: https://refactoring.com/catalog/replaceLoopWithPipeline.html
    title: Martin Fowler, Catalog of Refactorings, Replace Loop with Pipeline
    locator: the entry
  - book: isbn:9780134757599
    title: Martin Fowler, Refactoring, Improving the Design of Existing Code, second edition, Addison-Wesley, 2018
    locator: Chapter 8, Moving Features, Replace Loop with Pipeline, motivation and mechanics; Chapter 3, Bad Smells in Code, Loops
  - url: https://docs.python.org/3/tutorial/datastructures.html
    title: The Python Tutorial, Data Structures
    locator: 5.1.3 List Comprehensions
  - url: https://docs.oracle.com/en/java/javase/21/docs/api/java.base/java/util/stream/package-summary.html
    title: Java SE 21 API, package java.util.stream
    locator: Stream operations and pipelines; Laziness
  - url: https://doc.rust-lang.org/std/iter/index.html
    title: The Rust Standard Library, module std::iter
    locator: Adapters; Laziness
---

## Purpose

A loop says how to visit each item. What it computes is spread over the body: an `if` that skips some items, a line that changes each item, a `push` that collects the result. The reader must put those back together.

A collection pipeline names each step. In Fowler's words, "A collection pipeline lays out a sequence of operations that pass a collection of items between them".[^1] The catalogue card turns a loop that collects the names of programmers into `input.filter(...).map(...)`.[^2] The book names the [[loops-smell|Loops]] smell and gives this refactoring as its fix.[^3]

The pattern has a different form in each language: array methods in JavaScript, streams in Java, iterator adapters in Rust, and comprehensions or generator expressions in Python. The article notes that comprehensions express the same pipeline in another syntax.[^1] For Python, the tutorial puts it this way: "List comprehensions provide a concise way to create lists".[^4]

## Steps

1. Make a variable for the collection that the loop walks over.[^3]
2. Take the first piece of behavior in the loop body, for example the `if` that skips some items, and express it as a pipeline operation on that variable, here a `filter`. Test.
3. Repeat for each piece: a transformation becomes `map`, a nested collection becomes `flatMap`, a limit becomes `slice` or `take`.
4. When the loop body is empty, delete the loop. If the loop added to an accumulator, assign the pipeline's result to it.[^3]
5. Test.

## Example

A loop finds the paths of the three slowest failed requests in a log. The pipeline says the same thing as a sequence of steps.

```javascript run id=named-steps
import assert from "node:assert/strict";

const log = [
  { path: "/a", status: 200, ms: 40 }, { path: "/b", status: 503, ms: 900 },
  { path: "/c", status: 500, ms: 120 }, { path: "/d", status: 502, ms: 2500 },
  { path: "/e", status: 500, ms: 300 }, { path: "/f", status: 404, ms: 10 },
];

function slowFailuresLoop(log) {
  const failed = [];
  for (const r of log) {
    if (r.status >= 500) failed.push(r);
  }
  failed.sort((x, y) => y.ms - x.ms);
  const paths = [];
  for (let i = 0; i < failed.length && i < 3; i++) paths.push(failed[i].path);
  return paths;
}

const slowFailures = (log) => log
  .filter((r) => r.status >= 500)
  .toSorted((x, y) => y.ms - x.ms)
  .slice(0, 3)
  .map((r) => r.path);

assert.deepEqual(slowFailures(log), slowFailuresLoop(log));
assert.deepEqual(slowFailures(log), ["/d", "/b", "/e"]);
assert.equal(log[1].path, "/b"); // toSorted leaves the log in its order
console.log("replace loop with pipeline: tests pass");
```

```output
replace loop with pipeline: tests pass
```

Each line of the pipeline is one step: keep the failures, order by time, take three, keep the path. `toSorted` returns a sorted copy; `sort` would change the log in place.

| Language | Pipeline form | What the example shows |
|---|---|---|
| JavaScript | array methods | `filter`, `toSorted`, `slice` and `map` on an array, as above |
| Python | comprehensions and built-ins | a list comprehension, `sorted` with a key, and slicing; a generator expression for a lazy form |
| Java | streams | `stream().filter().sorted().limit().map().toList()`, with lazy intermediate operations |
| Rust | iterator adapters | `iter().filter().map()` and `collect`; the sort is on a collected `Vec`, since iterators do not sort |

## When to use

- **The loop filters, maps or collects.** Those steps have names in a pipeline.
- **The loop has one job.** A loop with two jobs first needs [[split-loop|Split Loop]].
- **The result is a value.** Pipelines fit computations that produce a collection or a value from a collection.

## Pitfalls

**Laziness.** In some languages the steps of a pipeline do not run until a final step asks for the result. In Java, "Intermediate operations are always lazy".[^5] In Rust, "Iterators (and iterator adapters) are lazy".[^6] Side effects inside a `map` then run late, or not at all if nothing reads the result. Keep pipeline steps free of side effects.

**Early exit and complex state.** A loop that stops at the first match, or that keeps state across items, maps to `find`, `takeWhile` or a fold. If no operation fits, the loop can be the clearer form.

**One pass or several.** A pipeline can make several passes, or build a collection at each step, where the loop made one. In most code this does not matter. If it does, measure, and use the language's lazy form: a stream in Java, an iterator in Rust, a generator expression in Python. See [[laziness|laziness]].

**Order.** A step that sorts or groups needs the whole collection. Put filters before it, so it sorts less.

## Tools that do it

This refactoring is a sequence of steps, not one action of a tool. Each step can be checked with the tests.

## References
