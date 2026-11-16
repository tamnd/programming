---
tag: 08RNHA
kind: feature
owner: python
slug: the-del-statement
name: The del statement
aliases: [del keyword, delete a variable, delete a list item, delete a dictionary key, del a slice, remove an attribute]
area: languages/python/control-flow
level: core
summary: >-
  del removes bindings. del x unbinds a name, del a[i] removes an item, del a[i:j] removes a slice and del obj.attr removes an attribute. It does not destroy the object itself.
grammar: "del_stmt: 'del' target_list (Language Reference, 7.5)"
spec: {standard: null, url: "https://docs.python.org/3.15/reference/simple_stmts.html#the-del-statement", locator: "7.5. The del statement"}
see_also: [python/assignment-statements, python/naming-and-binding, python/for-statements]
sources:
  - url: https://docs.python.org/3.15/reference/simple_stmts.html#the-del-statement
    title: The Python Language Reference, Python 3.15
    locator: 7.5. The del statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/tutorial/datastructures.html#the-del-statement
    title: The Python Tutorial, 5. Data Structures
    locator: 5.2. The del statement
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/reference/datamodel.html#object.__del__
    title: The Python Language Reference, Python 3.15
    locator: 3.3.1. Basic customization, object.__del__()
    accessed: 2026-11-16
  - url: https://go.dev/ref/spec#Deletion_of_map_elements
    title: The Go Programming Language Specification
    locator: Deletion of map elements
    accessed: 2026-11-16
---

## Synopsis

```python run id=list-items
a = [-1, 1, 66.25, 333, 333, 1234.5]
del a[0]
print(a)
del a[2:4]
print(a)
del a[:]
print(a)
```

```output for=list-items
[1, 66.25, 333, 333, 1234.5]
[1, 66.25, 1234.5]
[]
```

This is the example of the tutorial. `del` removes an item by its index, a slice, or all the items.

## Semantics

**r1.** "Deletion of a target list recursively deletes each target, from left to right."[^1] Example: [`targets`](#targets).

**r2.** "Deletion of a name removes the binding of that name from the local or global namespace, depending on whether the name occurs in a global statement in the same code block. Trying to delete an unbound name raises a NameError exception."[^1] Example: [`unbind`](#unbind).

**r3.** "Deletion of attribute references and subscriptions is passed to the primary object involved; deletion of a slicing is in general equivalent to assignment of an empty slice of the right type (but even this is determined by the sliced object)."[^1] Examples: [`list-items`](#list-items), [`targets`](#targets).

**r4.** The del statement "differs from the pop() method which returns a value."[^2]

**r5.** `del x` does not destroy the object. The data model says that "del x doesn’t directly call x.__del__()". The del statement "decrements the reference count for x by one", and `__del__()` "is only called when x’s reference count reaches zero."[^3] Example: [`other-name`](#other-name).

## Examples

```python run id=targets
d = {"a": 1, "b": 2, "c": 3}
nums = [10, 20, 30]

class Box:
    pass

box = Box()
box.size = 5
del d["a"], nums[-1], box.size
print(d, nums, hasattr(box, "size"))
```

```output for=targets
{'b': 2, 'c': 3} [10, 20] False
```

The dictionary removes a key, the list removes an item and the object removes an attribute (rule r3).

```python fails id=unbind expect=exception:NameError
x = 1
del x
print(x)
```

```python run id=other-name
a = [1, 2]
b = a
del a
print(b)
```

```output for=other-name
[1, 2]
```

`del a` removes only the name `a`. The list still exists, because `b` refers to it (rule r5).

## Pitfalls

`del` on a missing key raises an error:

```python fails id=missing-key expect=exception:KeyError
d = {"a": 1}
del d["b"]
```

Use `d.pop("b", None)` if the key can be missing.

A del of an index in a loop over the same list skips items, because the items after the deleted one move to a lower index:

```python wrong id=del-in-loop
nums = [1, 2, 2, 3]
for i, n in enumerate(nums):
    if n == 2:
        del nums[i]
print(nums)
```

```output for=del-in-loop
[1, 2, 3]
```

Build a new list, as in `nums = [n for n in nums if n != 2]`.

## Comparison

To remove a map entry, Go has a built-in function: "The built-in function delete removes the element with key k from a map m." Also, "If the map m is nil or the element m[k] does not exist, delete is a no-op."[^4] A Python `del d[k]` raises KeyError when the key does not exist, as in the `missing-key` example.
