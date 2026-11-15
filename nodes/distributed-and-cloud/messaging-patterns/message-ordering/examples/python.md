```python run id=resequence
# Messages arrive with a key and a sequence number for each key, out of order and with one copy.
arrivals = [("A", 1), ("A", 3), ("B", 1), ("A", 2), ("B", 3), ("A", 3), ("B", 2), ("A", 4)]

expected = {}  # key -> next sequence number to deliver
held = {}      # key -> {sequence number: message}
for key, seq in arrivals:
    nxt = expected.get(key, 1)
    name = "%s%d" % (key, seq)
    if seq < nxt or seq in held.get(key, {}):
        print("%s: copy, dropped" % name)
        continue
    held.setdefault(key, {})[seq] = name
    out = []
    while nxt in held[key]:
        out.append(held[key].pop(nxt))
        nxt += 1
    expected[key] = nxt
    if out:
        print("%s: deliver %s" % (name, " ".join(out)))
    else:
        print("%s: hold, waiting for %s%d" % (name, key, nxt))
left = sum(len(h) for h in held.values())
print("held at the end: %d; next: %s" % (left, ", ".join("%s%d" % kv for kv in sorted(expected.items()))))
```

```output
A1: deliver A1
A3: hold, waiting for A2
B1: deliver B1
A2: deliver A2 A3
B3: hold, waiting for B2
A3: copy, dropped
B2: deliver B2 B3
A4: deliver A4
held at the end: 0; next: A5, B4
```

- Verdict: idiom. Two dicts hold the state: `expected` maps each key to the next number to deliver, and `held` maps each key to the messages that came too early.
- `setdefault` makes the dict of held messages for a new key, and `pop` removes each message as the loop delivers it.
- A message whose number is below the next number, or that is already held, is a copy, so the resequencer also drops duplicates.
