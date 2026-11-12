```python run id=any-all-and-defaults
seen = []


def is_even(n):
    seen.append(n)
    return n % 2 == 0


nums = [1, 3, 4, 5, 7]
# any() and all() stop at the first value that decides the result.
print(any(is_even(n) for n in nums), seen)
seen.clear()
print(all(is_even(n) for n in nums), seen)
seen.clear()
# A list comprehension builds every value first, so nothing is skipped.
print(any([is_even(n) for n in nums]), seen)

# `or` treats 0 and "" as false, so it replaces values that are valid.
for retries in (None, 0, 3):
    print(retries, "->", retries or 5, retries if retries is not None else 5)

# A chained comparison is short-circuit too: 1 < 0 is false, so 1 / 0 is not evaluated.
print(1 < 0 < 1 / 0)
```

```output for=any-all-and-defaults
True [1, 3, 4]
False [1]
True [1, 3, 4, 5, 7]
None -> 5 5
0 -> 5 0
3 -> 3 3
False
```

- Verdict: native. `and` and `or` stop at the operand that decides the result and return that operand. `any` and `all` stop early when they read a generator, but a list comprehension computes every value first, so `is_even` runs five times. `or` treats `0` as false, so it replaces a valid zero; a test with `is not None` keeps it. A chained comparison such as `1 < 0 < 1 / 0` also stops at the first false part.
