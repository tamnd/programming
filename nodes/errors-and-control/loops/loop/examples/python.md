```python run id=for-else-builtins-and-recursion-depth
# for ... else: the else part runs when the loop ends without break.
def find(items, target):
    for i, x in enumerate(items):
        if x == target:
            print("found", target, "at", i)
            break
    else:
        print(target, "not found")


find([4, 8, 15], 8)
find([4, 8, 15], 16)

# A built-in function or a comprehension can replace a loop.
print(sum(range(1, 101)))
print([x * x for x in range(5)])


# Deep recursion fails, because Python does not remove tail calls.
def count(n):
    return 0 if n == 0 else 1 + count(n - 1)


try:
    count(100_000)
except RecursionError as e:
    print(type(e).__name__)

# The same repetition as a loop has no depth limit.
i = 0
while i < 100_000:
    i += 1
print(i)
```

```output for=for-else-builtins-and-recursion-depth
found 8 at 1
16 not found
5050
[0, 1, 4, 9, 16]
RecursionError
100000
```

- Verdict: native. Python has two loop statements, `while` and `for`. The `else` clause of the `for` runs only when the loop does not `break`, so it reports the failed search. `sum` and a list comprehension replace two short loops. A recursion 100 000 levels deep raises `RecursionError`, and a `while` loop with the same number of passes does not.
