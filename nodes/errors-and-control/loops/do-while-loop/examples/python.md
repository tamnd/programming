```python run id=while-true-with-the-test-at-the-end
def digits(n):
    # The body runs before the test, so 0 has one digit.
    count = 0
    while True:
        n //= 10
        count += 1
        if n == 0:
            break
    return count


print([digits(n) for n in (0, 7, 42, 1000)])

# A retry loop tries at least once and stops after a limit.
attempts = iter([False, False, True])
tries = 0
while True:
    tries += 1
    ok = next(attempts)
    if ok or tries >= 5:
        break
print("ok" if ok else "failed", "after", tries, "tries")
```

```output for=while-true-with-the-test-at-the-end
[1, 1, 2, 4]
ok after 3 tries
```

- Verdict: idiom. Python has no do-while loop, and PEP 315, which proposed one, was rejected. `while True:` with `if ...: break` as the last statement of the body is the post-test loop, so `digits(0)` is 1 and the retry loop makes at least one try.
