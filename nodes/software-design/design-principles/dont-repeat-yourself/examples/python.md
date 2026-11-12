```python run id=one-rate
VAT_RATE = 0.2


def tax_only(net: int) -> float:
    return net * VAT_RATE


def price_with_tax(net: int) -> float:
    return net + tax_only(net)


# Two facts that only look the same stay apart.
MAX_USERNAME_LENGTH = 20
MAX_LOGIN_ATTEMPTS = 20

print(price_with_tax(50), tax_only(50), MAX_USERNAME_LENGTH == MAX_LOGIN_ATTEMPTS)
```

```output
60.0 10.0 True
```
