```python run id=split-the-steps
# Before: one function, with comments for the steps.
def invoice_before(order):
    # add up the lines
    subtotal = 0
    for line in order["lines"]:
        subtotal += line["price"] * line["qty"]
    # discount for large orders
    discount = 0
    if subtotal >= 10000:
        discount = round(subtotal / 10)
    # tax on the discounted amount
    tax = round((subtotal - discount) / 5)
    # format
    total = subtotal - discount + tax
    return f"{order['customer']}: {total / 100:.2f}"


# After: each comment became the name of a function.
def subtotal(lines):
    return sum(line["price"] * line["qty"] for line in lines)


def discount(cents):
    return round(cents / 10) if cents >= 10000 else 0


def tax(cents):
    return round(cents / 5)


def format_cents(cents):
    return f"{cents / 100:.2f}"


def invoice(order):
    gross = subtotal(order["lines"])
    net = gross - discount(gross)
    return f"{order['customer']}: {format_cents(net + tax(net))}"


large = {"customer": "Ada", "lines": [{"price": 4000, "qty": 2}, {"price": 2500, "qty": 1}]}
small = {"customer": "Bob", "lines": [{"price": 1500, "qty": 2}]}
for f in (invoice_before, invoice):
    assert f(large) == "Ada: 113.40"
    assert f(small) == "Bob: 36.00"
    assert discount(9999) == 0
    print(f"{f.__name__}: tests pass")
```

```output
invoice_before: tests pass
invoice: tests pass
```

- Python's `round` rounds a half to the even neighbour, and JavaScript's `Math.round` rounds a half up. The amounts in the test divide exactly, so both give the same result. A real invoice must choose one rule and use it in one place, which the extracted `discount` and `tax` make possible.
- The local variable `subtotal` in the "before" version and the function `subtotal` in the "after" version have the same name. Inside the long function, the variable hides the function, which is one reason to extract before the names collide.
