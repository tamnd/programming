```python run id=clause-order-and-scope
matrix = [[1, 2, 3], [4, 5, 6]]

# Flatten: the for clauses read from left to right, like nested loops.
print([x for row in matrix for x in row])

# A comprehension inside a comprehension makes a nested list.
print([[row[i] for row in matrix] for i in range(3)])

# An "if" after the "for" is a filter. An "a if c else b" before the "for" is part of the expression.
nums = range(-2, 3)
print([n for n in nums if n > 0])
print([n if n > 0 else 0 for n in nums])

# An assignment expression in a comprehension binds its name in the enclosing scope.
data = [3, 8, 1, 9]
if any((big := n) > 5 for n in data):
    print("first big value:", big)


def source(name, items):
    print("evaluate", name)
    return items


# The leftmost source is evaluated at once. The other parts run when the generator runs.
gen = (x + y for x in source("outer", [1, 2]) for y in source("inner", [10]))
print("generator made")
print(list(gen))

# A generator can be used only once.
print(list(gen))
```

```output
[1, 2, 3, 4, 5, 6]
[[1, 4], [2, 5], [3, 6]]
[1, 2]
[0, 0, 0, 1, 2]
first big value: 8
evaluate outer
generator made
evaluate inner
evaluate inner
[11, 12]
[]
```

- Verdict: native. Python has list, set and dict comprehensions and generator expressions. This file shows a flatten and a nested comprehension, the difference between a filter and a conditional expression, an assignment expression that binds a name outside the comprehension, and a generator expression that evaluates its leftmost source at once and its other parts later. The second list() call gives an empty list because a generator can be read only once.
