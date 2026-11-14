```python run id=dict-and-ordereddict
from collections import OrderedDict

# dict keeps insertion order. Two dicts with the same pairs are equal in any order.
a = {"x": 1, "y": 2}
b = {"y": 2, "x": 1}
print(list(a), list(b), a == b)

# OrderedDict compares the order too.
oa, ob = OrderedDict(a), OrderedDict(b)
print(oa == ob, oa == b)

# move_to_end and popitem(last=False) reorder the keys in constant time.
od = OrderedDict.fromkeys("abcde")
od.move_to_end("b")
od.move_to_end("e", last=False)
print("".join(od))
print(od.popitem(last=False)[0], od.popitem()[0], "".join(od))

# A small least-recently-used cache: a hit moves the key to the end,
# and a full cache drops the first key.
class LRU:
    def __init__(self, size):
        self.size, self.data = size, OrderedDict()

    def get(self, k):
        if k not in self.data:
            return None
        self.data.move_to_end(k)
        return self.data[k]

    def put(self, k, v):
        self.data[k] = v
        self.data.move_to_end(k)
        if len(self.data) > self.size:
            self.data.popitem(last=False)


c = LRU(2)
c.put("a", 1)
c.put("b", 2)
c.get("a")
c.put("c", 3)
print(list(c.data), c.get("b"))

# Changing a dict while a loop reads it is an error.
d = {"a": 1}
try:
    for k in d:
        d["b"] = 2
except RuntimeError as e:
    print(type(e).__name__)
```

```output
['x', 'y'] ['y', 'x'] True
False True
eacdb
e b acd
['a', 'c'] None
RuntimeError
```

- Verdict: native. Since Python 3.7 a dict keeps insertion order. An update keeps the place of a key, and a key that is removed and added again goes to the end. collections.OrderedDict adds move_to_end and popitem(last=False), and its equality test compares the order. The example builds a small LRU cache with it.
