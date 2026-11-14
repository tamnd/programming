```python run id=counter
from collections import Counter

words = "the cat and the hat and the bat".split()
c = Counter(words)

# most_common gives the items with the highest counts first.
print(c.most_common(2))

# A missing item has the count 0, not a KeyError.
print(c["dog"], "dog" in c)

# Subtraction removes items whose count becomes zero or less.
print(Counter(a=3, b=1) - Counter(a=1, b=2))

# But setting a count to 0 keeps the item. Use del to remove it.
c["cat"] = 0
print("cat" in c, c.total())
del c["cat"]
print("cat" in c)

# update adds counts. subtract can make counts negative.
d = Counter(a=1)
d.update("aab")
d.subtract({"b": 3})
print(d)

# Is one multiset contained in another? Every count must be less or equal.
need, have = Counter("aab"), Counter("abca")
print(need <= have, Counter("aaa") <= have)
```

```output
[('the', 3), ('and', 2)]
0 False
Counter({'a': 2})
True 7
False
Counter({'a': 3, 'b': -2})
True False
```

- Verdict: stdlib. collections.Counter is a dict from item to count. A missing item counts 0, most_common gives the top items, and + - | & combine counters. Subtraction drops counts of zero or less, but a count set to 0 stays until del. <= tests that one multiset is inside another.
