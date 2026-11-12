```python run id=use-the-expression
# Before: a variable that adds no meaning.
def is_large_before(order):
    base_price = order["base_price"]
    return base_price > 1000

# After: the expression in place.
def is_large(order):
    return order["base_price"] > 1000

for p in (0, 1000, 1001):
    assert is_large({"base_price": p}) == is_large_before({"base_price": p})

# A variable that must stay: the list changes between the declaration and the use.
def total_with_fee(cart):
    subtotal = sum(cart)
    cart.append(5)  # the fee is an item too
    return subtotal + 5

def total_with_fee_wrong(cart):
    cart.append(5)
    return sum(cart) + 5  # inlined after the append

assert total_with_fee([10, 20]) == 35
assert total_with_fee_wrong([10, 20]) == 40
print("inline variable: tests pass")
```

```output
inline variable: tests pass
```
