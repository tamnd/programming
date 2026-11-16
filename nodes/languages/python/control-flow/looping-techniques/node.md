---
tag: 6XW2QD
kind: feature
owner: python
slug: looping-techniques
name: Looping Techniques
aliases: [enumerate, zip, reversed, sorted, loop with index, loop over two lists, iterate over dict items, zip strict]
area: languages/python/control-flow
level: intro
summary: >-
  Python has built-in helpers for common loops. items() gives keys and values, enumerate() gives indexes, zip() pairs sequences, and reversed() and sorted() change the order of the items.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/datastructures.html#looping-techniques", locator: "5.6. Looping Techniques"}
see_also: [python/for-statements, python/the-del-statement, python/break-and-continue-statements, python/iterable-unpacking]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#looping-techniques
    title: The Python Tutorial, 5. Data Structures
    locator: 5.6. Looping Techniques
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/builtins/functions.html#zip
    title: Built-in Functions, Python 3.15 documentation
    locator: zip(); enumerate()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#For_range
    title: The Go Programming Language Specification
    locator: For statements with range clause
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#the-range-function
    title: The Python Tutorial, 4. More Control Flow Tools
    locator: 4.3. The range() Function
    accessed: 2026-11-16
---

## Synopsis

```python run id=techniques
knights = {"gallahad": "the pure", "robin": "the brave"}
for k, v in knights.items():
    print(k, v)
for i, v in enumerate(["tic", "tac", "toe"]):
    print(i, v)
for q, a in zip(["name", "quest"], ["lancelot", "the holy grail"]):
    print(f"What is your {q}? It is {a}.")
```

```output for=techniques
gallahad the pure
robin the brave
0 tic
1 tac
2 toe
What is your name? It is lancelot.
What is your quest? It is the holy grail.
```

These are the examples of the tutorial.

## Semantics

**r1.** "When looping through dictionaries, the key and corresponding value can be retrieved at the same time using the items() method."[^1] Example: [`techniques`](#techniques).

**r2.** "When looping through a sequence, the position index and corresponding value can be retrieved at the same time using the enumerate() function."[^1] Example: [`techniques`](#techniques).

**r3.** "To loop over two or more sequences at the same time, the entries can be paired with the zip() function."[^1] "By default, zip() stops when the shortest iterable is exhausted."[^2] With `strict=True`, "it raises a ValueError if one iterable is exhausted before the others".[^2] Examples: [`techniques`](#techniques), [`zip-strict`](#zip-strict).

**r4.** "To loop over a sequence in reverse, first specify the sequence in a forward direction and then call the reversed() function."[^1] Example: [`order`](#order).

**r5.** "To loop over a sequence in sorted order, use the sorted() function which returns a new sorted list while leaving the source unaltered."[^1] "The use of sorted() in combination with set() over a sequence is an idiomatic way to loop over unique elements of the sequence in sorted order."[^1] Example: [`order`](#order).

**r6.** "It is sometimes tempting to change a list while you are looping over it; however, it is often simpler and safer to create a new list instead."[^1] Example: [`filter`](#filter).

## Examples

```python run id=order
for i in reversed(range(1, 10, 2)):
    print(i)
basket = ["apple", "orange", "apple", "pear", "orange", "banana"]
for f in sorted(set(basket)):
    print(f)
print(basket)
```

```output for=order
9
7
5
3
1
apple
banana
orange
pear
['apple', 'orange', 'apple', 'pear', 'orange', 'banana']
```

`sorted()` does not change `basket` (rule r5).

```python run id=filter
import math

raw_data = [56.2, float("NaN"), 51.7, 55.3, 52.5, float("NaN"), 47.8]
filtered_data = []
for value in raw_data:
    if not math.isnan(value):
        filtered_data.append(value)
print(filtered_data)
```

```output for=filter
[56.2, 51.7, 55.3, 52.5, 47.8]
```

```python run id=enumerate-start
for n, line in enumerate(["first", "second"], start=1):
    print(n, line)
```

```output for=enumerate-start
1 first
2 second
```

## Pitfalls

`zip()` without `strict=True` drops the extra items with no error:

```python wrong id=zip-short
names = ["a", "b", "c"]
scores = [1, 2]
print(list(zip(names, scores)))
```

```output for=zip-short
[('a', 1), ('b', 2)]
```

The documentation warns: "Without the strict=True argument, any bug that results in iterables of different lengths will be silenced".[^2] Pass `strict=True` when the lengths must be equal:

```python fails id=zip-strict expect=exception:ValueError
names = ["a", "b", "c"]
scores = [1, 2]
for pair in zip(names, scores, strict=True):
    print(pair)
```

A loop over `range(len(seq))` works, but `enumerate(seq)` gives the index and the item together. The tutorial says: "In most such cases, however, it is convenient to use the enumerate() function".[^4]

## Comparison

Go has one loop form for most of these cases, the range clause. A for statement with a range clause "iterates through all entries of an array, slice, string or map, values received on a channel", and more.[^3] "For each entry it assigns iteration values to corresponding iteration variables if present and then executes the block."[^3] A Go range over a slice gives the index and the value, as `enumerate()` does in Python. A Go range over a map gives the key and the value, as `items()` does.
