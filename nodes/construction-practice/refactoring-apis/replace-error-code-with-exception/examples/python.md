```python run id=code-to-exception
country_data = {"shipping_rules": {"PT": {"flat": 5}}}


def local_shipping_rules_before(country: str):
    data = country_data["shipping_rules"].get(country)
    return data if data else -23


class OrderProcessingError(Exception):
    def __init__(self, code: int) -> None:
        super().__init__(f"order processing error {code}")
        self.code = code


def local_shipping_rules(country: str) -> dict:
    data = country_data["shipping_rules"].get(country)
    if data:
        return data
    raise OrderProcessingError(-23)


def shipping_cost(country: str) -> int:
    return local_shipping_rules(country)["flat"]


assert local_shipping_rules_before("XX") == -23
status = []
for country in ["PT", "XX"]:
    try:
        status.append(shipping_cost(country))
    except OrderProcessingError as e:
        status.append(e.code)
assert status == [5, -23]
print(status)
```

```output
[5, -23]
```
