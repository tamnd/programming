```python wrong id=denylist
PRICES = {"pen": 150, "book": 1200}  # in cents
BLOCKED = ("<", ">", "'", '"', ";", "--")


def order_total(product, quantity):
    for bad in BLOCKED:
        if bad in product or bad in quantity:
            raise ValueError("blocked character")
    return PRICES[product] * int(quantity)


print(order_total("book", "2"))
print(order_total("book", "-3"))
print(order_total("book", " 2\n"))
print(order_total("book", "٣"))  # ARABIC-INDIC DIGIT THREE
```

```output for=denylist
2400
-3600
2400
3600
```

```python run id=parse
import re
from dataclasses import dataclass

PRICES = {"pen": 150, "book": 1200}  # in cents
QUANTITY = re.compile(r"[0-9]{1,3}")


@dataclass(frozen=True)
class Order:
    product: str
    quantity: int


def parse_order(form):
    product = form.get("product", "")
    if product not in PRICES:
        raise ValueError("unknown product")
    text = form.get("quantity", "")
    if not QUANTITY.fullmatch(text):
        raise ValueError("quantity must be 1 to 3 ASCII digits")
    quantity = int(text)
    if not 1 <= quantity <= 100:
        raise ValueError("quantity must be from 1 to 100")
    return Order(product, quantity)


def order_total(order: Order) -> int:
    return PRICES[order.product] * order.quantity


forms = [
    {"product": "book", "quantity": "2"},
    {"product": "book", "quantity": "-3"},
    {"product": "book", "quantity": " 2\n"},
    {"product": "book", "quantity": "٣"},
    {"product": "book", "quantity": "0"},
    {"product": "boat", "quantity": "1"},
]
for form in forms:
    try:
        print(order_total(parse_order(form)))
    except ValueError as error:
        print("rejected:", error)
```

```output for=parse
2400
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be 1 to 3 ASCII digits
rejected: quantity must be from 1 to 100
rejected: unknown product
```

```python wrong id=check-then-normalize
import unicodedata

MAX_LENGTH = 10


def save_name(raw):
    if len(raw) > MAX_LENGTH:
        raise ValueError("name too long")
    name = unicodedata.normalize("NFKC", raw)
    return name


name = save_name("\ufb03" * 10)
print(len(name), name)
```

```output for=check-then-normalize
30 ffiffiffiffiffiffiffiffiffiffi
```

```python run id=normalize-then-check
import unicodedata

MAX_LENGTH = 10


def save_name(raw):
    name = unicodedata.normalize("NFKC", raw)
    if len(name) > MAX_LENGTH:
        raise ValueError("name too long")
    return name


print(save_name("Zoe"))
try:
    save_name("\ufb03" * 10)
except ValueError as error:
    print("rejected:", error)
```

```output for=normalize-then-check
Zoe
rejected: name too long
```

- Verdict: stdlib. Python has no validation framework in the standard library, but `re.fullmatch`, `int`, `dataclasses` and `unicodedata` are enough to write a parser for each field.
- `int` accepts surrounding white space, an underscore between digits, a sign, and any Unicode decimal digit. Check the text with a pattern before the conversion when the field must have one exact form.
- In a `str` pattern, `\d` matches any Unicode decimal digit, and `$` also matches before a trailing newline. Use `[0-9]` and `fullmatch`, or compile with `re.ASCII`.
- A frozen `dataclass` keeps a parsed value from changing later. Python does not stop other code from calling `Order(...)` with bad values, so the rule "build it only with the parser" is a convention. Add a check in `__post_init__` to enforce it.
- The ligature `\ufb03` (`ﬃ`) is one character. NFKC turns it into three letters. Ten of them pass the length check of the first function and come out as a name of 30 characters. The second function normalizes first and checks the value it stores.
