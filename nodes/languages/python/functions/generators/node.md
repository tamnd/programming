---
tag: 1RPCD9
kind: feature
owner: python
slug: generators
name: Generators and generator functions
aliases: [generator, generator function, generator iterator, yield statement, lazy sequence, StopIteration, write an iterator with yield]
area: languages/python/functions
level: core
summary: >-
  A function that contains yield is a generator function. A call returns a generator iterator and runs no code. Each next() runs the body to the next yield, and the end of the body raises StopIteration.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#generator-types", locator: "Generator types; Generator iterators"}
see_also: [python/yield-expressions, python/generator-expressions, python/return-statement, python/for-statements, python/defining-functions]
sources:
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#generator-types
    title: Built-in Types, Python 3.15 documentation
    locator: Iterator types; Generator types; Generator iterators
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/classes.html#generators
    title: The Python Tutorial, 9. Classes
    locator: 9.9. Generators
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#generator-functions
    title: The Python Language Reference, Python 3.15
    locator: 3.2.8.3. Generator functions
    accessed: 2026-11-16
  - url: https://peps.python.org/pep-0255/
    title: "PEP 255: Simple Generators"
    locator: "Specification: Yield"
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
    accessed: 2026-11-16
---

## Synopsis

```python run id=countdown
def countdown(n):
    while n > 0:
        yield n
        n -= 1

for k in countdown(3):
    print(k)
print(list(countdown(5)))
```

```output for=countdown
3
2
1
[5, 4, 3, 2, 1]
```

`countdown` gives its values one at a time. The `for` loop and `list()` ask for the next value until the function ends.

## Semantics

**r1.** "A function that contains one or more yield expressions is a generator function."[^1] "Generator functions behave as regular user-defined functions (for example, they have the same attributes), except that calling a generator function returns a generator iterator".[^1]

**r2.** "When a generator function is called, the actual arguments are bound to function-local formal argument names in the usual way, but no code in the body of the function is executed."[^4] Example: [`lazy`](#lazy).

**r3.** "Each time next() is called on it, the generator resumes where it left off (it remembers all the data values and which statement was last executed)."[^2] Example: [`lazy`](#lazy).

**r4.** "When the function executes a return statement or falls off the end, a StopIteration exception is raised and the iterator will have reached the end of the set of values to be returned."[^3] Example: [`next-after-end`](#next-after-end).

**r5.** "Once an iterator’s __next__() method raises StopIteration, it must continue to do so on subsequent calls."[^1] So a generator iterator gives its values only once. Example: [`second-pass`](#second-pass).

**r6.** "What makes generators so compact is that the __iter__() and __next__() methods are created automatically."[^2] "One common use for generator functions is implementing the __iter__() method of custom iterable objects."[^1] Example: [`iter-method`](#iter-method).

## Examples

```python run id=lazy
def numbers():
    print("start")
    yield 1
    print("after 1")
    yield 2
    print("end")

g = numbers()
print(type(g).__name__)
print(next(g))
print(next(g))
print(next(g, "done"))
```

```output for=lazy
generator
start
1
after 1
2
end
done
```

The call `numbers()` prints nothing (rule r2). Each `next()` runs the body to the next `yield` (rule r3). The default argument of `next()` is returned when the generator ends.

```python run id=running-total
def running_total(values):
    total = 0
    for v in values:
        total += v
        yield total

print(list(running_total([3, 1, 4, 1, 5])))
```

```output for=running-total
[3, 4, 8, 9, 14]
```

The local variable `total` keeps its value between the values that the generator gives (rule r3).

```python run id=iter-method
class Deck:
    def __init__(self, cards):
        self.cards = cards

    def __iter__(self):
        for card in self.cards:
            yield card.upper()

deck = Deck(["ace", "king"])
print(list(deck))
print(list(deck))
```

```output for=iter-method
['ACE', 'KING']
['ACE', 'KING']
```

Each `for` or `list()` calls `__iter__` again and gets a new generator, so the object can be iterated many times (rule r6).

```python run id=infinite
def naturals():
    n = 0
    while True:
        yield n
        n += 1

for n in naturals():
    if n * n > 50:
        break
    print(n, end=" ")
print()
```

```output for=infinite
0 1 2 3 4 5 6 7
```

The generator makes a value only when the loop asks for it, so an endless generator is safe when the loop stops.

## Pitfalls

A generator iterator is used up after one pass:

```python wrong id=second-pass
def squares(n):
    for i in range(n):
        yield i * i

sq = squares(4)
print(sum(sq))
print(sum(sq))
```

```output for=second-pass
14
0
```

The second `sum` gets no values (rule r5). Call `squares(4)` again, or keep the values in a list.

A `next()` call with no default after the end raises StopIteration:

```python fails id=next-after-end expect=exception:StopIteration
def one():
    yield 1

g = one()
next(g)
next(g)
```

## Comparison

Go has no `yield` keyword, but a `for` loop can range over a function: "For a function f, the iteration proceeds by calling f with a new, synthesized yield function as its argument."[^5] The Go function calls `yield` for each value, and `yield` returns false when the loop body stops. A Python generator function stops at each `yield` and continues at the next `next()` call (rule r3).
