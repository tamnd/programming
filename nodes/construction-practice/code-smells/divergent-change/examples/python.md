```python run id=split-phase-report
from dataclasses import dataclass


# Before: the computation and the format are mixed.
def report_before(sales, fmt):
    total = 0
    by_region = {}
    out = "region,amount\n" if fmt == "csv" else ""
    for s in sales:
        total += s["amount"]
        by_region[s["region"]] = by_region.get(s["region"], 0) + s["amount"]
    for region, amount in by_region.items():
        out += f"{region},{amount}\n" if fmt == "csv" else f"{region}: {amount}\n"
    out += f"total,{total}\n" if fmt == "csv" else f"Total: {total}\n"
    return out


# After: phase 1 computes a summary, phase 2 renders it.
@dataclass(frozen=True)
class Summary:
    by_region: dict
    total: int


def summarize(sales):
    by_region = {}
    for s in sales:
        by_region[s["region"]] = by_region.get(s["region"], 0) + s["amount"]
    return Summary(by_region, sum(by_region.values()))


def render_text(summary):
    lines = [f"{r}: {a}\n" for r, a in summary.by_region.items()]
    return "".join(lines) + f"Total: {summary.total}\n"


def render_csv(summary):
    lines = [f"{r},{a}\n" for r, a in summary.by_region.items()]
    return "region,amount\n" + "".join(lines) + f"total,{summary.total}\n"


RENDERERS = {"text": render_text, "csv": render_csv}


def report(sales, fmt):
    return RENDERERS[fmt](summarize(sales))


sales = [
    {"region": "north", "amount": 100},
    {"region": "south", "amount": 50},
    {"region": "north", "amount": 25},
]
for fmt in ("text", "csv"):
    assert report(sales, fmt) == report_before(sales, fmt)
assert report(sales, "text") == "north: 125\nsouth: 50\nTotal: 175\n"
assert summarize(sales).total == 175
print("report: tests pass")
```

```output
report: tests pass
```

- A Python `dict` keeps the order of insertion, so the regions come out in the order they first appear in the sales, in both versions.
- The dataclass is frozen, so a renderer cannot change the summary that another renderer also reads.
- A new format is one function and one entry in `RENDERERS`. The computation and its tests do not change.
