```python run id=comprehension-partial-and-callbacks
from functools import partial

# A comprehension of lambdas shares one variable too.
fs = [lambda: i for i in range(3)]
print([f() for f in fs])

# A default argument copies the value of each pass.
fs = [lambda i=i: i for i in range(3)]
print([f() for f in fs])

# functools.partial stores the argument when it is made.
def greet(name):
    return "hi " + name

calls = [partial(greet, n) for n in ["ann", "bob"]]
print([c() for c in calls])

# Callbacks that run after the loop see the last value.
handlers = {}
for key in ["a", "b", "c"]:
    handlers[key] = lambda: key.upper()
print({k: h() for k, h in handlers.items()})
```

```output for=comprehension-partial-and-callbacks
[2, 2, 2]
[0, 1, 2]
['hi ann', 'hi bob']
{'a': 'C', 'b': 'C', 'c': 'C'}
```

- Verdict: idiom. Python has one variable for each function or comprehension, so all the lambdas of a comprehension read the same `i`, and all the handlers read the same `key`, after the loop has left `"c"` in it. The default argument `i=i` and `functools.partial` take the value at the time they are made.
