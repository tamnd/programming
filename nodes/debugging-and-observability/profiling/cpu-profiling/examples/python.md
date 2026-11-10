```python run id=cprofile
import cProfile
import pstats


def fib(n):
    return n if n < 2 else fib(n - 1) + fib(n - 2)


def price(item):
    return {"apple": 30, "bread": 250}.get(item, 0)


def total(cart):
    return sum(price(item) for item in cart)


def work():
    fib(15)
    for _ in range(100):
        total(["apple", "bread", "apple"])


profiler = cProfile.Profile()
profiler.runcall(work)
stats = pstats.Stats(profiler)

# Each key is (file, line, function). Each value starts with the number of
# primitive calls and the number of all calls. Print counts, not times.
print(f"{'function':<10} {'ncalls':>9}")
for (file, line, name), (primitive, calls, *_times) in sorted(stats.stats.items(), key=lambda kv: kv[0][2]):
    if file == __file__:
        shown = f"{calls}/{primitive}" if calls != primitive else str(calls)
        print(f"{name:<10} {shown:>9}")
```

```output for=cprofile
function      ncalls
<genexpr>        400
fib           1973/1
price            300
total            100
work               1
```

A profiler that counts calls can also count paths. `sys.setprofile` calls a function at each call and each return. The tracer below keeps a stack of names and counts each path. The result is the folded format that flame graph tools read.

```python run id=folded
import sys
from collections import Counter

stack = []
folded = Counter()


def tracer(frame, event, arg):
    """Records each call as a path of function names, the input of a flame graph."""
    if event == "call":
        stack.append(frame.f_code.co_name)
        folded[";".join(stack)] += 1
    elif event == "return":
        stack.pop()


def parse(line):
    return line.split(",")


def check(fields):
    return all(fields)


def load(lines):
    return [check(parse(line)) for line in lines]


def main():
    load(["a,b", "c,d", "e,"])
    check(["x"])


sys.setprofile(tracer)
main()
sys.setprofile(None)
for path, count in sorted(folded.items()):
    print(path, count)
```

```output for=folded
main 1
main;check 1
main;load 1
main;load;check 3
main;load;parse 3
```

`Profile.run` and `cProfile.run` take a string of code. It runs the string in the namespace of the module `__main__`, not in the namespace of the caller. A local variable of the caller is not visible. `runctx` takes the namespaces as arguments, and `runcall` takes a function.

```python fails expect=exception:NameError id=run-locals
import cProfile


def fib(n):
    return n if n < 2 else fib(n - 1) + fib(n - 2)


def main():
    n = 20
    print("profiling fib(n) with n =", n)
    profiler = cProfile.Profile()
    profiler.run("fib(n)")  # the string runs in the namespace of __main__


main()
```

```output for=run-locals
profiling fib(n) with n = 20
```

- Verdict: stdlib. `cProfile` and `profile` are deterministic profilers: they record every call and every return.
- `pstats.Stats` sorts and prints the results. A count such as `1973/1` means 1973 calls, of which 1 was a primitive call, not a recursive call.
- The examples print only call counts. The times change on each run.
- `python -m cProfile -s cumtime script.py` profiles a whole script from the command line.
