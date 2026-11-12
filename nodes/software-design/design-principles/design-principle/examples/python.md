```python run id=two-copies
# The rule "orders of 50 or more ship free" is written twice.
def banner_text(total: int) -> str:
    return "Free shipping" if total >= 50 else "Shipping: 5"


# The business changes the limit to 40. Only one copy is updated.
def shipping_cost(total: int) -> int:
    return 0 if total >= 40 else 5


print(shipping_cost(45), banner_text(45))
```

```output
0 Shipping: 5
```
