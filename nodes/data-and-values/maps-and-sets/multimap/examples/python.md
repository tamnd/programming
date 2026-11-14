```python run id=defaultdict-list
from collections import defaultdict
from urllib.parse import parse_qs

# defaultdict(list) makes the empty list for a new key.
pairs = [("yellow", 1), ("blue", 2), ("yellow", 3), ("blue", 4), ("red", 1)]
d = defaultdict(list)
for k, v in pairs:
    d[k].append(v)
print(dict(d))

# Reading a missing key with [] adds it. Use get or in to only read.
print(d.get("green"), "green" in d)
_ = d["green"]
print("green" in d, d["green"])

# Remove one value. Remove the key too when its list becomes empty.
d["red"].remove(1)
if not d["red"]:
    del d["red"]
print(sorted(d))

# A set as the value keeps each value once for each key.
tags = defaultdict(set)
for k, v in [("a", "x"), ("a", "x"), ("a", "y")]:
    tags[k].add(v)
print(sorted(tags["a"]))

# A query string is a multimap: one name can have many values.
print(parse_qs("color=red&size=9&color=blue"))
```

```output
{'yellow': [1, 3], 'blue': [2, 4], 'red': [1]}
None False
True []
['blue', 'green', 'yellow']
['x', 'y']
{'color': ['red', 'blue'], 'size': ['9']}
```

- Verdict: idiom. A dict of lists, built with setdefault or collections.defaultdict(list). Reading a missing key of a defaultdict with [] adds it. A set as the value keeps each value once. urllib.parse.parse_qs reads a query string into a dict of lists.
