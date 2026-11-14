```python run id=set-and-frozenset
# {} is an empty dict. An empty set needs set().
print(type({}).__name__, type(set()).__name__)

# remove raises KeyError for a missing value, and discard does not.
s = {1, 2, 3}
s.discard(9)
try:
    s.remove(9)
except KeyError:
    print("KeyError")

# Elements must be hashable. A frozenset can be an element, and a set cannot.
pairs = {frozenset({1, 2}), frozenset({2, 1})}
print(len(pairs))
try:
    {{1, 2}}
except TypeError as e:
    print(type(e).__name__)

# Equal values are one element: 1, 1.0 and True are equal.
print(len({1, 1.0, True}))

# A set removes duplicates but loses the order. dict.fromkeys keeps the first order.
words = ["b", "a", "b", "c", "a"]
print(sorted(set(words)), list(dict.fromkeys(words)))

# The methods take any iterable, and the operators need sets.
print(sorted(s.union([4, 5])))
try:
    s | [4, 5]
except TypeError as e:
    print(type(e).__name__)

# A frozenset cannot change.
f = frozenset(s)
print(hasattr(f, "add"), f == s)
```

```output
dict set
KeyError
1
TypeError
1
['a', 'b', 'c'] ['b', 'a', 'c']
[1, 2, 3, 4, 5]
TypeError
False True
```

- Verdict: native. set is mutable and frozenset is immutable and hashable. {} is an empty dict, so an empty set needs set(). remove raises KeyError and discard does not. 1, 1.0 and True are one element. The methods such as union take any iterable, and the operators such as | need sets.
