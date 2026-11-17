---
tag: BY6PSM
kind: feature
owner: python
slug: changing-a-list-while-iterating
name: Changing a list while iterating
aliases: [modifying a list while iterating, remove in a for loop, dictionary changed size during iteration, set changed size during iteration, B909, loop-iterator-mutation]
area: languages/python/pitfalls
level: intro
summary: >-
  A loop that removes items from the list that it iterates over skips items. A dict or set that changes size in a loop raises RuntimeError. Loop over a copy, or build a new collection.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/controlflow.html#for-statements", locator: "4.2 for Statements"}
proposals: []
see_also: [python/for-statements, python/lists, python/dictionaries, python/sets, python/dict-order-and-views, python/list-comprehensions, python/copying-collections]
sources:
  - url: https://docs.python.org/3.15/tutorial/controlflow.html#for-statements
    title: The Python Tutorial, Python 3.15 documentation
    locator: 4.2 for Statements
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/reference/compound_stmts.html#the-for-statement
    title: The Python Language Reference, Python 3.15 documentation
    locator: 8.3 The for statement
    accessed: 2026-11-18
  - url: https://docs.astral.sh/ruff/rules/loop-iterator-mutation/
    title: loop-iterator-mutation (B909), Ruff documentation
    locator: What it does; Why is this bad?
    accessed: 2026-11-18
  - url: https://docs.python.org/3.15/library/stdtypes.html#dictionary-view-objects
    title: Built-in Types, Python 3.15 documentation
    locator: Dictionary view objects
    accessed: 2026-11-18
---

## Synopsis

```python run id=skips
numbers = [1, 2, 2, 3, 4, 4, 5]
for n in numbers:
    if n % 2 == 0:
        numbers.remove(n)
print(numbers)

numbers = [1, 2, 2, 3, 4, 4, 5]
odd = [n for n in numbers if n % 2 != 0]
print(odd)
```

```output for=skips
[1, 2, 3, 4, 5]
[1, 3, 5]
```

The first loop left a 2 and a 4 in the list. The comprehension builds a new list and does not change the old one.

## Semantics

**r1.** The tutorial warns: "Code that modifies a collection while iterating over that same collection can be tricky to get right. Instead, it is usually more straight-forward to loop over a copy of the collection or to create a new collection".[^1]

**r2.** A `for` loop gets its items from an iterator: "An iterator is created for that iterable. The first item provided by the iterator is then assigned to the target list".[^2] The iterator of a list keeps a position. When the loop removes the item at the position, the next item moves into that position, and the iterator steps past it. Example: [`skips`](#skips).

**r3.** An insert at or before the position makes the loop see an item twice. An append in the loop can make the loop run forever, because the list gets longer at each step. Ruff explains that such mutation "can lead to unexpected behavior, like skipping elements or infinite loops."[^3] Example: [`append-grows`](#append-grows).

**r4.** Dictionaries and sets check their size. "Iterating views while adding or deleting entries in the dictionary may raise a RuntimeError or fail to iterate over all entries."[^4] In CPython, a change of size raises `RuntimeError` at the next step. Example: [`dict-size`](#dict-size).

**r5.** A change of a value is safe. A loop can assign `d[key] = new_value` for a key that is already there, and it can assign `items[i] = x`, because the size does not change.

**r6.** The fixes: loop over a copy, such as `items[:]`, `list(d)` or `list(d.items())`; build a new collection with a comprehension; or collect the changes in the loop and apply them after it. Example: [`fixes`](#fixes).

**r7.** Ruff rule B909 "Checks for mutations to an iterable during a loop iteration."[^3]

## Examples

```python run id=fixes
tasks = ["a", "skip", "skip", "b"]
for t in tasks[:]:
    if t == "skip":
        tasks.remove(t)
print(tasks)

prices = {"rice": 2, "pho": 0, "tea": 0}
for name in list(prices):
    if prices[name] == 0:
        del prices[name]
print(prices)

stock = {"a": 1, "b": 2}
stock = {k: v * 10 for k, v in stock.items()}
print(stock)
```

```output for=fixes
['a', 'b']
{'rice': 2}
{'a': 10, 'b': 20}
```

`tasks[:]` and `list(prices)` are copies, so the loop does not see the changes (rule r6). The last part makes a new dictionary.

```python run id=reverse-index
items = [5, -1, 3, -2, -7, 8]
for i in range(len(items) - 1, -1, -1):
    if items[i] < 0:
        del items[i]
print(items)
```

```output for=reverse-index
[5, 3, 8]
```

A loop from the end to the start can delete in place, because a deletion moves only the items after the position, and the loop has already seen them.

## Versions

The behavior is the same in all Python 3 versions. The dict and set iterators check the size, and the list iterator does not.

## Pitfalls

```python fails id=dict-size expect=exception:RuntimeError
ages = {"ann": 31, "bob": 17, "cy": 15}
for name in ages:
    if ages[name] < 18:
        del ages[name]
```

The first deletion changes the size, so the next step of the loop raises `RuntimeError: dictionary changed size during iteration` (rule r4). Loop over `list(ages)`.

```python wrong id=append-grows
queue = [1, 2, 3]
seen = 0
for job in queue:
    seen += 1
    if job < 4:
        queue.append(job + 3)
    if seen > 20:
        break
print(seen, queue)
```

```output for=append-grows
6 [1, 2, 3, 4, 5, 6]
```

Each new job is also a job for the loop (rule r3). Here a test stops the growth after six jobs. With no such test, the loop never ends. If a queue that grows is the intent, use `collections.deque` and a `while` loop, so that the intent is clear.

## Comparison

Some languages make all their collection iterators fail fast when the collection changes. Python checks only dicts and sets. A list change in a loop gives no error, only wrong results (rule r2). For the views of a dictionary, see [[python/dict-order-and-views]]. For copies, see [[python/copying-collections]].
