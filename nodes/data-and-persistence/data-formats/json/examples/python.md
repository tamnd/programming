```python run id=parse-emit
import json
from decimal import Decimal

text = '{"id": 9007199254740993, "total": 0.10, "tags": ["new"], "paid": false, "note": null}'

order = json.loads(text)
print(order)
print(json.dumps(order, separators=(",", ":"), sort_keys=True))
print(json.dumps({"city": "Hà Nội"}))
print(json.dumps({"city": "Hà Nội"}, ensure_ascii=False))

exact = json.loads(text, parse_float=Decimal)
print(repr(exact["total"]))
```

```output for=parse-emit
{'id': 9007199254740993, 'total': 0.1, 'tags': ['new'], 'paid': False, 'note': None}
{"id":9007199254740993,"note":null,"paid":false,"tags":["new"],"total":0.1}
{"city": "H\u00e0 N\u1ed9i"}
{"city": "Hà Nội"}
Decimal('0.10')
```

```python run id=round-trip
import json
from datetime import date

before = {1: "one", "pair": (1, 2), "ratio": float("nan")}
after = json.loads(json.dumps(before))
print(after)

try:
    json.dumps({"day": date(2026, 11, 10)})
except TypeError as error:
    print("TypeError:", error)

print(json.loads('{"total": 1, "total": 2}'))
```

```output for=round-trip
{'1': 'one', 'pair': [1, 2], 'ratio': nan}
TypeError: Object of type date is not JSON serializable
{'total': 2}
```

```python run id=strict
import json


def reject_duplicates(pairs):
    names = [name for name, _ in pairs]
    if len(names) != len(set(names)):
        raise ValueError(f"duplicate names in {names}")
    return dict(pairs)


def reject_constant(name):
    raise ValueError(f"{name} is not JSON")


for text in ['{"total": 1, "total": 2}', "[NaN]"]:
    try:
        json.loads(text, object_pairs_hook=reject_duplicates, parse_constant=reject_constant)
    except ValueError as error:
        print(error)

try:
    json.dumps(float("inf"), allow_nan=False)
except ValueError as error:
    print(error)
```

```output for=strict
duplicate names in ['total', 'total']
NaN is not JSON
Out of range float values are not JSON compliant: inf
```

- Verdict: stdlib. The `json` module parses with `json.loads` (a string) and `json.load` (a file), and writes with `json.dumps` and `json.dump`.
- A JSON integer becomes an `int` of any size, so `9007199254740993` survives. A JSON number with a fraction or an exponent becomes a `float`. `parse_float=Decimal` keeps the exact decimal text.
- By default `dumps` escapes every non-ASCII character as `\uXXXX`. `ensure_ascii=False` writes the characters as they are.
- The defaults accept and write `NaN`, `Infinity` and `-Infinity`, which are not JSON. `allow_nan=False` and `parse_constant` turn them into errors.
- A repeated name keeps the last value. `object_pairs_hook` sees every pair and can reject the object.
- An integer key becomes a string, a tuple becomes a list, and a `date` needs a `default` function or a conversion to an ISO 8601 string first.
- An integer with more than 4300 digits raises `ValueError`, because CPython limits the length of integer strings to protect against slow conversions.
