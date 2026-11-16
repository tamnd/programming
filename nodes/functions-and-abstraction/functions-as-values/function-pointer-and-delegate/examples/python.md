```python run id=function-pointer-and-delegate-python
import operator


def add(a, b):
    return a + b


def apply(f, a, b):
    return f(a, b)


op = add
print(op(6, 7))
op = operator.mul
print(op(6, 7))
print(apply(lambda a, b: a - b, 6, 7))
```

```output for=function-pointer-and-delegate-python
13
42
-1
```

- Verdict: native. A Python variable holds the function object itself, so no pointer type is needed. `operator.mul` is a built-in function that a variable can hold in the same way.
