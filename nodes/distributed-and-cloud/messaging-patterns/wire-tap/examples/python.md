```python run id=tap-audit
import copy
from collections import deque


class WireTap:
    """A fixed recipient list: each message goes to the main channel and, as a copy, to the tap."""

    def __init__(self, main, tap):
        self.main, self.tap, self.enabled = main, tap, True

    def send(self, msg):
        if self.enabled:
            self.tap.append(copy.deepcopy(msg))  # the tap must not share objects with the main flow
        self.main.append(msg)


main_ch, audit_ch = deque(), deque()
wire = WireTap(main_ch, audit_ch)
orders = [("o1", "4111111111111111", 30), ("o2", "5500000000000004", 12),
          ("o3", "4111111111111111", 7), ("o4", "340000000000009", 45)]
for oid, card, total in orders:
    wire.send({"id": oid, "card": card, "total": total, "headers": {"source": "web"}})
    if oid == "o3":
        wire.enabled = False  # an operator turns the tap off; the main flow does not change

seen = []
while main_ch:
    m = main_ch.popleft()
    seen.append(m)
    print("main: %s card %s total %d" % (m["id"], m["card"], m["total"]))
audited = 0
while audit_ch:
    m = audit_ch.popleft()
    m["card"] = "*" * (len(m["card"]) - 4) + m["card"][-4:]
    m["headers"]["audit"] = "yes"
    audited += 1
    print("audit: %s card %s total %d" % (m["id"], m["card"], m["total"]))
print("main: %d messages, audit: %d messages (tap off after o3)" % (len(seen), audited))
unchanged = all("*" not in m["card"] and "audit" not in m["headers"] for m in seen)
print("main messages unchanged:", str(unchanged).lower())
```

```output
main: o1 card 4111111111111111 total 30
main: o2 card 5500000000000004 total 12
main: o3 card 4111111111111111 total 7
main: o4 card 340000000000009 total 45
audit: o1 card ************1111 total 30
audit: o2 card ************0004 total 12
audit: o3 card ************1111 total 7
main: 4 messages, audit: 3 messages (tap off after o3)
main messages unchanged: true
```

- Verdict: idiom. Two `deque` objects stand for the channels, and the tap is a small class with one `send` method.
- `copy.deepcopy` gives the audit channel its own `headers` dict; `dict(msg)` would share it.
- The `enabled` flag plays the part of an operator who turns the tap off; the main channel gets all 4 orders.
