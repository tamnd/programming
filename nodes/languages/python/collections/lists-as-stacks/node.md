---
tag: BNKMSK
kind: feature
owner: python
slug: lists-as-stacks
name: Using lists as stacks
aliases: [stack, list as stack, LIFO, last-in first-out, push and pop, append and pop, stack in Python]
area: languages/python/collections
level: intro
summary: >-
  A list works as a stack: append() pushes an item on the top and pop() with no index takes the item from the top. The last item added is the first item taken out.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/tutorial/datastructures.html#using-lists-as-stacks", locator: "5.1.1. Using Lists as Stacks"}
see_also: [python/lists, python/lists-as-queues, python/the-collections-module]
sources:
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#using-lists-as-stacks
    title: The Python Tutorial, 5. Data Structures
    locator: 5.1. More on Lists; 5.1.1. Using Lists as Stacks
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/collections.html#collections.deque
    title: collections, Python 3.15 documentation
    locator: deque objects
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Appending_and_copying_slices
    title: The Go Programming Language Specification
    locator: Appending to and copying slices
    accessed: 2026-11-16
---

## Synopsis

```python run id=push-pop
stack = [3, 4, 5]
stack.append(6)
stack.append(7)
print(stack)
print(stack.pop(), stack.pop())
print(stack, stack[-1])
```

```output for=push-pop
[3, 4, 5, 6, 7]
7 6
[3, 4, 5] 5
```

The end of the list is the top of the stack. `stack[-1]` reads the top item and does not remove it.

## Semantics

**r1.** A stack is a structure "where the last element added is the first element retrieved (“last-in, first-out”). To add an item to the top of the stack, use append(). To retrieve an item from the top of the stack, use pop() without an explicit index."[^1]

**r2.** pop() is described so: "Remove the item at the given position in the list, and return it. If no index is specified, a.pop() removes and returns the last item in the list. It raises an IndexError if the list is empty or the index is outside the list range."[^1] Example: [`empty-pop`](#empty-pop).

**r3.** The deque documentation says that list objects "are optimized for fast fixed-length operations and incur O(n) memory movement costs for pop(0) and insert(0, v) operations".[^2] A stack uses only the end of the list, so it does not move the other items.

## Examples

```python run id=brackets
def balanced(text):
    pairs = {")": "(", "]": "[", "}": "{"}
    stack = []
    for ch in text:
        if ch in "([{":
            stack.append(ch)
        elif ch in pairs:
            if not stack or stack.pop() != pairs[ch]:
                return False
    return not stack

print(balanced("(a[b]{c})"), balanced("(]"), balanced("(("))
```

```output for=brackets
True False False
```

Each closing bracket must match the last open bracket, so a stack is the right structure (rule r1).

```python run id=undo
history = []
text = ""
for word in ["one", "two", "three"]:
    history.append(text)
    text += word + " "
print(repr(text))
text = history.pop()
text = history.pop()
print(repr(text))
```

```output for=undo
'one two three '
'one '
```

Each `pop()` gives back the state before the last change.

## Pitfalls

pop() on an empty list is an error:

```python fails id=empty-pop expect=exception:IndexError
stack = []
stack.pop()
```

Test the list first with `if stack:` (rule r2).

`pop(0)` takes the bottom item, not the top item. A loop of `pop(0)` calls on a long list is slow (rule r3). Use `collections.deque` for a queue.

## Comparison

In Go, a slice can also work as a stack. "The variadic function append appends zero or more values x to a slice s of type S and returns the resulting slice, also of type S."[^3] To pop, a Go program reads `s[len(s)-1]` and then sets `s = s[:len(s)-1]`. A Python list does both steps in one `pop()` call (rule r2).
