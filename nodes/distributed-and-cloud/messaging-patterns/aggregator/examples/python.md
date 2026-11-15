```python run id=aggregate-parts
# Messages from a splitter: (correlation id, sequence number, sequence size, item).
incoming = [
    ("A", 2, 3, "gadget"),
    ("B", 1, 2, "widget"),
    ("A", 1, 3, "widget"),
    ("B", 2, 2, "gizmo"),
    ("A", 2, 3, "gadget"),  # a second copy, as at-least-once delivery can give
    ("A", 3, 3, "widget"),
    ("B", 1, 2, "widget"),  # a copy that comes after B is complete
]

groups = {}    # correlation id -> {sequence number: item}
closed = set()

for cid, seq, size, item in incoming:
    if cid in closed:
        print(f"late: {cid} {seq}/{size} discarded, the group is closed")
        continue
    parts = groups.setdefault(cid, {})
    if seq in parts:
        print(f"duplicate: {cid} {seq}/{size} ignored")
        continue
    parts[seq] = item
    if len(parts) == size:  # the completion condition: all parts are here
        items = [parts[n] for n in sorted(parts)]
        print(f"{cid} complete: {', '.join(items)}")
        del groups[cid]
        closed.add(cid)

print(f"still open: {len(groups)}")
```

```output
B complete: widget, gizmo
duplicate: A 2/3 ignored
A complete: widget, gadget, widget
late: B 1/2 discarded, the group is closed
still open: 0
```

- Verdict: idiom. A dict from the correlation id to a dict of the parts is the store of groups, and `setdefault` makes a group on the first part.
- The parts are kept by sequence number, so a second copy of a part does not count twice, and `sorted(parts)` gives the items in order.
- The set `closed` keeps the ids of the complete groups, so a late copy is discarded and does not start a new group; in a long-running process, this set needs a limit or a time to live.
