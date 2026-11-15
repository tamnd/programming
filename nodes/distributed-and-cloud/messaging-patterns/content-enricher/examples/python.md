```python run id=enrich-orders
from datetime import datetime, timedelta, timezone

ZIP_PREFIX = {"981": "WA", "100": "NY", "606": "IL"}   # used to compute the state
DIRECTORY = {"c1": "Ada Park", "c2": "Lin Tran"}       # the other system


class Clock:  # the environment; a fixed start keeps the output the same each run
    def __init__(self):
        self.t = datetime(2026, 1, 5, 9, 0, 0, tzinfo=timezone.utc)

    def now(self):
        self.t += timedelta(seconds=1)
        return self.t


class Enricher:
    def __init__(self, clock):
        self.clock = clock
        self.lookups = 0

    def enrich(self, msg):
        self.lookups += 1
        name = DIRECTORY.get(msg["customer"])
        if name is None:
            return None, "customer %s not found" % msg["customer"]
        out = dict(msg)  # keep the fields of the original message
        out["name"] = name
        out["state"] = ZIP_PREFIX.get(msg["zip"][:3], "??")
        out["received"] = self.clock.now().strftime("%H:%M:%S")
        return out, None


incoming = [
    {"order": "o1", "customer": "c1", "zip": "98101"},
    {"order": "o2", "customer": "c2", "zip": "10001"},
    {"order": "o3", "customer": "c1", "zip": "60601"},
    {"order": "o4", "customer": "c9", "zip": "98101"},
]
enricher = Enricher(Clock())
enriched = invalid = 0
for msg in incoming:
    out, err = enricher.enrich(msg)
    if err:
        invalid += 1
        print("invalid: %s %s" % (msg["order"], err))
        continue
    enriched += 1
    print("%s %s %s, %s %s, received %s" % (out["order"], out["customer"], out["name"],
                                           out["zip"], out["state"], out["received"]))
print("lookups: %d, enriched: %d, invalid: %d" % (enricher.lookups, enriched, invalid))
```

```output
o1 c1 Ada Park, 98101 WA, received 09:00:01
o2 c2 Lin Tran, 10001 NY, received 09:00:02
o3 c1 Ada Park, 60601 IL, received 09:00:03
invalid: o4 customer c9 not found
lookups: 4, enriched: 3, invalid: 1
```

- Verdict: idiom. A dict stands for the customer directory, `dict(msg)` keeps the original fields, and the enricher adds three new ones.
- The clock is an object that the enricher gets in its constructor, so a test can give it a fixed time.
- A miss returns an error in place of a half-filled message, so the caller can route it.
