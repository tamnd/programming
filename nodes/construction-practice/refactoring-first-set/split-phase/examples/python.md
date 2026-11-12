```python run id=two-phases
from dataclasses import dataclass

INPUT = "ana,38,2500\nbo,45,2000\ncy,40,3000"

@dataclass(frozen=True)
class Timesheet:  # the intermediate structure between the phases
    name: str
    hours: int
    rate_cents: int

# Phase 1: text in, records out.
def parse_timesheets(text):
    sheets = []
    for line in text.splitlines():
        name, hours, rate = line.split(",")
        sheets.append(Timesheet(name, int(hours), int(rate)))
    return sheets

# Phase 2: records in, pay out. Integer cents; overtime is paid at 1.5 times.
def compute_pay(sheets):
    pay = {}
    for s in sheets:
        overtime = max(0, s.hours - 40)
        pay[s.name] = (s.hours - overtime) * s.rate_cents + overtime * s.rate_cents * 3 // 2
    return pay

result = compute_pay(parse_timesheets(INPUT))
assert result == {"ana": 95000, "bo": 95000, "cy": 120000}
assert compute_pay([Timesheet("dee", 42, 1000)]) == {"dee": 43000}
print(result)
print("split phase: tests pass")
```

```output
{'ana': 95000, 'bo': 95000, 'cy': 120000}
split phase: tests pass
```
