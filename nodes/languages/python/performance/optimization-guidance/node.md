---
tag: SR3BHQ
kind: feature
owner: python
slug: optimization-guidance
name: Optimization guidance
aliases: [optimize Python, speed up Python, str.join, string concatenation, functools.cache, memoization, bisect, hot loop, micro-optimization]
area: languages/python/performance
level: core
summary: >-
  Measure first, then fix the algorithm and the data structure. Use the built-ins, join strings with str.join, cache repeated work and move fixed lookups out of hot loops.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3/faq/programming.html#my-program-is-too-slow-how-do-i-speed-it-up", locator: "Programming FAQ, My program is too slow. How do I speed it up?"}
see_also: [python/performance-measurement, python/the-cost-model-of-cpython]
sources:
  - url: https://docs.python.org/3/faq/programming.html#my-program-is-too-slow-how-do-i-speed-it-up
    title: Programming FAQ
    locator: My program is too slow. How do I speed it up?; What is the most efficient way to concatenate many strings together?
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/stdtypes.html#common-sequence-operations
    title: Built-in Types
    locator: Common Sequence Operations, note 6
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/functools.html#functools.cache
    title: functools, Higher-order functions and operations on callable objects
    locator: functools.cache; functools.lru_cache
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/bisect.html
    title: bisect, Array bisection algorithm
    locator: introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3/library/dis.html
    title: dis, Disassembler for Python bytecode
    locator: LOAD_FAST; LOAD_ATTR
    accessed: 2026-11-16
  - url: https://pkg.go.dev/strings#Builder
    title: Package strings
    locator: type Builder
    accessed: 2026-11-16
---

## Synopsis

```python run id=join
import io

words = ['spam'] * 5

chunks = []
for w in words:
    chunks.append(w)
print('-'.join(chunks))

buf = io.StringIO()
for w in words:
    buf.write(w)
print(buf.getvalue())

data = bytearray()
for w in words:
    data += w.encode()
print(bytes(data))
```

```output for=join
spam-spam-spam-spam-spam
spamspamspamspamspam
b'spamspamspamspamspam'
```

To make a program faster, first measure it and find the hot spot. Then change the algorithm or the data structure, because this gives the largest gains. After that, small changes can help: use the built-in functions, join strings with `str.join`, cache work that repeats, and keep fixed lookups out of hot loops. The example shows the three idioms that the FAQ recommends to build a long string or a long bytes object.

## Semantics

**r1.** "You should always find the hot spots in your program before attempting to optimize any code (see the profile module). Writing benchmark scripts will allow you to iterate quickly when searching for improvements (see the timeit module)."[^1]

**r2.** "Making your algorithms faster (or changing to faster ones) can yield much larger benefits than trying to sprinkle micro-optimization tricks all over your code."[^1] "Use the right data structures. Study documentation for the Built-in Types and the collections module."[^1] Example: [`algorithm`](#algorithm).

**r3.** "When the standard library provides a primitive for doing something, it is likely (although not guaranteed) to be faster than any alternative you may come up with. This is doubly true for primitives written in C, such as builtins and some extension types."[^1]

**r4.** "Concatenating immutable sequences always results in a new object. This means that building up a sequence by repeated concatenation will have a quadratic runtime cost in the total sequence length."[^2] "To accumulate many str objects, the recommended idiom is to place them into a list and call str.join() at the end".[^1] "(Another reasonably efficient idiom is to use io.StringIO.)"[^1] "To accumulate many bytes objects, the recommended idiom is to extend a bytearray object using in-place concatenation (the += operator)".[^1] Examples: [`join`](#join) and [`copies`](#copies).

**r5.** `functools.cache` is a "Simple lightweight unbounded function cache. Sometimes called “memoize”."[^3] Example: [`cache`](#cache).

**r6.** The functions of `bisect` find a place in a sorted list. "For long lists of items with expensive comparison operations, this can be an improvement over linear searches or frequent resorting."[^4] Example: [`algorithm`](#algorithm).

**r7.** In a function, `LOAD_FAST` "Pushes a reference to the local co_varnames[var_num] onto the stack."[^5] `LOAD_ATTR` "replaces STACK[-1] with getattr(STACK[-1], co_names[namei>>1])".[^5] An attribute that the loop reads in each pass is a lookup in each pass. A local name that holds the value before the loop removes it. Example: [`hoist`](#hoist).

**r8.** "Abstractions tend to create indirections and force the interpreter to work more. If the levels of indirection outweigh the amount of useful work done, your program will be slower. You should avoid excessive abstraction, especially under the form of tiny functions or methods (which are also often detrimental to readability)."[^1]

## Examples

A model of the copies. With `s = s + piece`, each step copies the whole string again. With `join`, each character is copied once (rule r4):

```python run id=copies
def copied_by_concat(pieces):
    total, length = 0, 0
    for p in pieces:
        length += len(p)
        total += length
    return total

def copied_by_join(pieces):
    return sum(len(p) for p in pieces)

for n in (10, 100, 1000):
    pieces = ['abcd'] * n
    print(n, copied_by_concat(pieces), copied_by_join(pieces))
```

```output for=copies
10 220 40
100 20200 400
1000 2002000 4000
```

A better algorithm does less work. In a sorted list of 1000 items, `bisect` and the last `==` make 11 comparisons. The `in` operator compares each item up to the one that it finds, so it makes 1000 (rules r2 and r6):

```python run id=algorithm
import bisect

class Num(int):
    compares = 0

    def __lt__(self, other):
        Num.compares += 1
        return int(self) < int(other)

    def __eq__(self, other):
        Num.compares += 1
        return int(self) == int(other)

    __hash__ = int.__hash__

items = [Num(i) for i in range(1000)]
target = Num(999)

Num.compares = 0
print(target in items, Num.compares)

Num.compares = 0
i = bisect.bisect_left(items, target)
print(i < len(items) and items[i] == target, Num.compares)
```

```output for=algorithm
True 1000
True 11
```

`functools.cache` keeps each result, so a recursive function computes each value once (rule r5):

```python run id=cache
import functools

@functools.cache
def fib(n):
    return n if n < 2 else fib(n - 1) + fib(n - 2)

print(fib(80))
print(fib.cache_info())
```

```output for=cache
23416728348467685
CacheInfo(hits=78, misses=81, maxsize=None, currsize=81)
```

The second loop reads the attribute `out.append` once, before the loop. The bytecode of its loop body has no `LOAD_ATTR` (rule r7):

```python run id=hoist
import dis

def slow(xs):
    out = []
    for x in xs:
        out.append(x * 2)
    return out

def fast(xs):
    out = []
    append = out.append
    for x in xs:
        append(x * 2)
    return out

def attrs_in_loop(f):
    ins = list(dis.get_instructions(f))
    start = next(i for i, x in enumerate(ins) if x.opname == 'FOR_ITER')
    return sum(1 for x in ins[start:] if x.opname.startswith('LOAD_ATTR'))

print(slow([1, 2, 3]) == fast([1, 2, 3]))
print(attrs_in_loop(slow), attrs_in_loop(fast))
```

```output for=hoist
True
1 0
```

## Pitfalls

Do not optimize code that is not hot (rule r1). A change in a part of the program that takes 1% of the time can save at most 1% of the time.

Do not build a long string with `+=` in a loop (rule r4). Use `str.join`, `io.StringIO` or a `bytearray`.

`functools.cache` keeps each result for ever. It never evicts a value, so a cache on a function with many different arguments grows without a limit. Use `functools.lru_cache` with a `maxsize` when the arguments vary much. The arguments must be hashable.

A change such as a local name for a method makes the code harder to read (rule r8). Make it only in a hot loop, and measure the gain.

Keep tests for the code before you optimize it. The FAQ says: "It is highly recommended to have good code coverage (through unit testing or any other technique) before potentially introducing regressions hidden in sophisticated optimizations."[^1]

## Comparison

Go has the same problem with strings, because a Go string is also immutable. The Go answer is `strings.Builder`: "A Builder is used to efficiently build a string using Builder.Write methods. It minimizes memory copying. The zero value is ready to use."[^6] In Python, the list and `str.join`, or `io.StringIO`, do this job (rule r4).

## References

[^1]: [Programming FAQ](https://docs.python.org/3/faq/programming.html#my-program-is-too-slow-how-do-i-speed-it-up)
[^2]: [Built-in Types](https://docs.python.org/3/library/stdtypes.html#common-sequence-operations)
[^3]: [functools, Higher-order functions and operations on callable objects](https://docs.python.org/3/library/functools.html#functools.cache)
[^4]: [bisect, Array bisection algorithm](https://docs.python.org/3/library/bisect.html)
[^5]: [dis, Disassembler for Python bytecode](https://docs.python.org/3/library/dis.html)
[^6]: [Package strings](https://pkg.go.dev/strings#Builder)
