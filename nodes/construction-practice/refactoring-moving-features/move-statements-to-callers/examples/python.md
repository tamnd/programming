```python run id=let-callers-vary
items = [("Tea", 350), ("Scone", 275)]


def receipt_old(items):
    lines = [f"{n} {c / 100:.2f}" for n, c in items]
    lines.append("Thank you!")
    return lines


# Step 2: extract what stays, with a temporary name.
def receipt_lines(items):
    return [f"{n} {c / 100:.2f}" for n, c in items]


# Step 3: inline the old function into each caller; the footer is now theirs.
till = receipt_lines(items) + ["Thank you!"]
email = receipt_lines(items) + ["Thank you!", "Unsubscribe: example.com/u"]

# Step 4: rename receipt_lines back to the old name.
receipt = receipt_lines

assert till == receipt_old(items)
assert email[:3] == receipt_old(items) and email[-1].startswith("Unsubscribe")
assert receipt(items) == ["Tea 3.50", "Scone 2.75"]
print("move statements to callers: tests pass")
```

```output
move statements to callers: tests pass
```
