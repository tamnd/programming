```python run id=transaction-script
# A thin wrapper over the stored tables. The scripts call it directly.
db = {"customers": {1: {"name": "ada", "credit": 5000}}, "orders": {}}


def find_customer(customer_id):
    return db["customers"][customer_id]


def insert_order(customer_id, cents):
    order_id = len(db["orders"]) + 1
    db["orders"][order_id] = {"customer": customer_id, "cents": cents, "state": "open"}
    return order_id


def money(cents):
    return f"{cents // 100}.{cents % 100:02d}"


# One transaction script for each request from the presentation.
def place_order(customer_id, cents):
    customer = find_customer(customer_id)
    if customer["credit"] < cents:
        return f"refused, credit left {money(customer['credit'])}"
    customer["credit"] -= cents
    return f"order {insert_order(customer_id, cents)}"


def cancel_order(order_id):
    order = db["orders"][order_id]
    if order["state"] != "open":
        return "already cancelled"
    order["state"] = "cancelled"
    find_customer(order["customer"])["credit"] += order["cents"]
    return f"refunded {money(order['cents'])}"


print("place_order(ada, 30.00):", place_order(1, 3000))
print("place_order(ada, 30.00):", place_order(1, 3000))
print("cancel_order(1):", cancel_order(1))
print("cancel_order(1):", cancel_order(1))
print("place_order(ada, 30.00):", place_order(1, 3000))
print("credit of ada:", money(find_customer(1)["credit"]))
```

```output
place_order(ada, 30.00): order 1
place_order(ada, 30.00): refused, credit left 20.00
cancel_order(1): refunded 30.00
cancel_order(1): already cancelled
place_order(ada, 30.00): order 2
credit of ada: 20.00
```

- Verdict: idiom. Each script is a plain function, and the tables are dictionaries behind small finder and insert functions.
