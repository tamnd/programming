```python run id=for-else
def first_negative(rows):
    for r, row in enumerate(rows):
        for c, value in enumerate(row):
            if value < 0:
                return r, c
    return None


print(first_negative([[3, 1], [4, -1], [-5, 9]]))

for n in [9, 15, 21]:
    if n % 2 == 0:
        print("found an even number:", n)
        break
else:
    print("no even number")
```

```output for=for-else
(1, 1)
no even number
```

- Verdict: native. Python has blocks, `if`, `while`, `for`, `break`, `continue`, `return` and exceptions, and no `goto`. The design FAQ states that exceptions can act as a structured `goto` that works across function calls.
- To leave nested loops, move them into a function and `return`. The `else` clause of a loop runs only when the loop ends without `break`.
