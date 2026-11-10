```python run id=invoice
def invoice_before(customer, items):
    lines = [f"Invoice for {customer}"]
    for name, price, quantity in items:
        lines.append(f"  {name}: {quantity} x {price // 100}.{price % 100:02d}")
    # compute the total
    total = 0
    for _, price, quantity in items:
        total += price * quantity
    lines.append(f"Total: {total // 100}.{total % 100:02d}")
    return "\n".join(lines)


def format_cents(cents):
    return f"{cents // 100}.{cents % 100:02d}"


def total_cents(items):
    return sum(price * quantity for _, price, quantity in items)


def invoice_after(customer, items):
    lines = [f"Invoice for {customer}"]
    for name, price, quantity in items:
        lines.append(f"  {name}: {quantity} x {format_cents(price)}")
    lines.append(f"Total: {format_cents(total_cents(items))}")
    return "\n".join(lines)


ITEMS = [("pen", 250, 2), ("pad", 1205, 1)]
EXPECTED = "Invoice for Ada\n  pen: 2 x 2.50\n  pad: 1 x 12.05\nTotal: 17.05"

for invoice in (invoice_before, invoice_after):
    assert invoice("Ada", ITEMS) == EXPECTED
    assert invoice("Bob", []) == "Invoice for Bob\nTotal: 0.00"
    print(f"{invoice.__name__}: tests pass")
print(invoice_after("Ada", ITEMS))
```

```output for=invoice
invoice_before: tests pass
invoice_after: tests pass
Invoice for Ada
  pen: 2 x 2.50
  pad: 1 x 12.05
Total: 17.05
```

- Verdict: idiom. The refactoring needs only a function definition. The same tests run on the code before and after the change, and both pass.
- A fragment that assigns several variables can return a tuple, and the caller unpacks it: `count, total = add_up(items)`.
- A fragment that reads a variable of an enclosing function can become a nested function, which is a closure. A module-level function with parameters is clearer and can have its own tests.
- PyCharm and the rope library do this refactoring as one command.
