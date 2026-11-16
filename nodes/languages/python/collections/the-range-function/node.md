---
tag: 9FNEF4
kind: feature
owner: python
slug: the-range-function
name: The range() function
aliases: [range, range(), range object, arithmetic progression, loop n times, range with step, xrange]
area: languages/python/collections
level: intro
summary: >-
  range(stop) and range(start, stop, step) give an immutable sequence of integers. The stop value is not part of it. A range object computes its items, so it takes the same small memory for any length.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/builtins/stdtypes.html#ranges", locator: "Ranges"}
see_also: [python/for-statements, python/sequence-types, python/lists, python/looping-techniques]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#the-range-function
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.3. The range() Function
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/stdtypes.html#ranges
    title: Built-in Types, Python 3.15 documentation
    locator: Ranges
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
    accessed: 2026-11-16
---

## Synopsis

```python run id=progressions
for i in range(3):
    print(i)
print(list(range(5, 10)))
print(list(range(0, 10, 3)))
print(list(range(-10, -100, -30)))
```

```output for=progressions
0
1
2
[5, 6, 7, 8, 9]
[0, 3, 6, 9]
[-10, -40, -70]
```

`range(3)` gives 0, 1 and 2. The second and third arguments set the start and the step.

## Semantics

**r1.** "The range type represents an immutable sequence of numbers and is commonly used for looping a specific number of times in for loops."[^2]

**r2.** "The given end point is never part of the generated sequence; range(10) generates 10 values, the legal indices for items of a sequence of length 10."[^1]

**r3.** "If the step argument is omitted, it defaults to 1. If the start argument is omitted, it defaults to 0. If step is zero, ValueError is raised."[^2] Example: [`zero-step`](#zero-step).

**r4.** "For a positive step, the contents of a range r are determined by the formula r[i] = start + step*i where i >= 0 and r[i] < stop."[^2] "A range object will be empty if r[0] does not meet the value constraint."[^2] Example: [`empty`](#empty).

**r5.** "The advantage of the range type over a regular list or tuple is that a range object will always take the same (small) amount of memory, no matter the size of the range it represents (as it only stores the start, stop and step values, calculating individual items and subranges as needed)."[^2]

**r6.** "Ranges implement all of the common sequence operations except concatenation and repetition".[^2] Example: [`sequence-ops`](#sequence-ops).

**r7.** "Testing range objects for equality with == and != compares them as sequences."[^2]

## Examples

```python run id=sequence-ops
r = range(0, 20, 2)
print(r, len(r))
print(11 in r, 10 in r)
print(r.index(10), r[5], r[-1])
print(r[:5])
```

```output for=sequence-ops
range(0, 20, 2) 10
False True
5 10 18
range(0, 10, 2)
```

A slice of a range is a range (rule r6).

```python run id=indices
names = ["Mary", "had", "a", "little", "lamb"]
for i in range(len(names)):
    print(i, names[i])
```

```output for=indices
0 Mary
1 had
2 a
3 little
4 lamb
```

`range(len(names))` gives each valid index (rule r2). The function `enumerate()` gives the index and the item together.

```python run id=equal
print(range(0) == range(2, 1, 3))
print(range(0, 3, 2) == range(0, 4, 2))
print(list(range(0, 4, 2)))
```

```output for=equal
True
True
[0, 2]
```

Two ranges are equal when they give the same values (rule r7).

## Pitfalls

A start that is past the stop gives an empty range, not an error:

```python wrong id=empty
for i in range(5, 0):
    print(i)
print("done")
```

```output for=empty
done
```

To count down, give a negative step: `range(5, 0, -1)` (rule r4).

A step of zero is an error:

```python fails id=zero-step expect=exception:ValueError
range(1, 10, 0)
```

## Comparison

Go has no range type. A Go `for` loop can range over an integer: "For an integer value n, where n is of integer type or an untyped integer constant, the iteration values 0 through n-1 are produced in increasing order."[^3] A step other than 1 needs a three-clause `for` loop. A Python range is an object that a program can store, index and slice (rule r6).
