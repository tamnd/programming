```python run id=always-together
ana = {"name": "Ana Pires", "address": {"street": "Rua Augusta 10", "city": "Lisboa", "postcode": "1100-053"}}


# Before: two callers write the name before each call.
def address_lines(a):
    return [a["street"], f"{a['postcode']} {a['city']}"]


label_before = [ana["name"], *address_lines(ana["address"])]
invoice_before = ["Bill to:", ana["name"], *address_lines(ana["address"])]


# Step 3: extract the call and the name together, with a temporary name.
def address_block_tmp(r):
    return [r["name"], *address_lines(r["address"])]


# Step 5: inline address_lines into it. Step 6: rename to address_block.
def address_block(r):
    a = r["address"]
    return [r["name"], a["street"], f"{a['postcode']} {a['city']}"]


assert address_block_tmp(ana) == label_before
assert address_block(ana) == label_before
assert ["Bill to:", *address_block(ana)] == invoice_before
print("move statements into function: tests pass")
```

```output
move statements into function: tests pass
```
