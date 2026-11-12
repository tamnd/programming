```python run id=receipt-email
sent: list[str] = []


def send_receipt(order: dict[str, str]) -> None:
    sent.append(f"email to {order['email']}: {order['total']}")


send_receipt({"email": "ana@example.org", "total": "12.50"})
print(sent[0])
```

```output
email to ana@example.org: 12.50
```
