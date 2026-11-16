```python run id=generator-fringe
def walk(tree):
    if isinstance(tree, list):
        for child in tree:
            yield from walk(child)  # each level of the recursion is a generator
    else:
        yield tree


def same_fringe(x, y):
    gx, gy = walk(x), walk(y)
    while True:
        a, b = next(gx, None), next(gy, None)
        if a != b:
            return False
        if a is None:
            return True


a = [[1, 2], [3, [4]]]
b = [1, [2, [3, 4]]]
c = [[1, 2], [3, 5]]

for leaf in walk(a):
    print("got", leaf)
print("same fringe a b:", str(same_fringe(a, b)).lower())
print("same fringe a c:", str(same_fringe(a, c)).lower())
```

```output for=generator-fringe
got 1
got 2
got 3
got 4
same fringe a b: true
same fringe a c: false
```

- Verdict: absent. Python has no fibers in the standard library. Generators are the alternative, but they are stackless: only the generator function itself can suspend.
- To suspend inside a recursion, each level must be a generator, and `yield from` passes the values of the inner generator up to the outer one.
- `same_fringe` advances two generators in turn and stops at the first difference. The generators that it does not finish stay suspended until they are garbage-collected.
