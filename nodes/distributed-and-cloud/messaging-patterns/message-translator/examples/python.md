```python run id=translate-orders
import re

# Lines in the format of a partner: id;customer;amount;day/month/year
partner = [
    "P-17;Lan Pham;12.50;03/11/2026",
    "P-18;Hoa Le;8;05/11/2026",
    "P-19;Minh Tran;7,25;04/11/2026",
]


def translate(line):
    """Translate one partner line into the fields of the shop."""
    pid, customer, amount, date = line.split(";")
    m = re.fullmatch(r"(\d+)(?:\.(\d\d))?", amount)
    if m is None:
        raise ValueError("bad amount " + amount)
    cents = int(m.group(1)) * 100 + int(m.group(2) or "0")
    day, month, year = date.split("/")
    return {"id": int(pid.removeprefix("P-")), "customer": customer,
            "total_cents": cents, "date": f"{year}-{month}-{day}"}


for line in partner:
    try:
        o = translate(line)
    except ValueError as err:
        print(f"invalid: {line} ({err})")
        continue
    print(f"order {o['id']}: customer={o['customer']} total_cents={o['total_cents']} date={o['date']}")
```

```output
order 17: customer=Lan Pham total_cents=1250 date=2026-11-03
order 18: customer=Hoa Le total_cents=800 date=2026-11-05
invalid: P-19;Minh Tran;7,25;04/11/2026 (bad amount 7,25)
```

- Verdict: idiom. A function from the fields of one format to a dict of the other format is the translator, and `re.fullmatch` checks the amount before the code changes it to cents.
- The translator raises `ValueError` for a line that it cannot read, and the loop sends that line to the invalid channel with the reason.
- With a broker, the translator is a consumer of the partner queue and a producer for the queue of the shop.
