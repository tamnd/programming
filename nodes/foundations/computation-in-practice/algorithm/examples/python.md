```python run id=bisect-and-insort
import bisect

scores = [12, 31, 47, 47, 58, 90]
print("bisect_left 47:", bisect.bisect_left(scores, 47))
print("bisect_right 47:", bisect.bisect_right(scores, 47))
print("bisect_left 50:", bisect.bisect_left(scores, 50), "(where 50 would go)")

bisect.insort(scores, 50)  # finds the place in O(log n) steps, then shifts the tail
print("after insort:", scores)

people = [("Ada", 1815), ("Alan", 1912), ("Grace", 1906)]
people.sort(key=lambda p: p[1])  # stable: equal keys keep their order
i = bisect.bisect_left(people, 1900, key=lambda p: p[1])
print("born from 1900:", [p[0] for p in people[i:]])
```

```output for=bisect-and-insort
bisect_left 47: 2
bisect_right 47: 4
bisect_left 50: 4 (where 50 would go)
after insort: [12, 31, 47, 47, 50, 58, 90]
born from 1900: ['Grace', 'Alan']
```

```python wrong id=bisect-unsorted
import bisect

scores = [90, 12, 58, 31, 47]  # not sorted
i = bisect.bisect_left(scores, 90)
print("index:", i, "found:", i < len(scores) and scores[i] == 90)
print("90 in scores:", 90 in scores)
```

```output for=bisect-unsorted
index: 5 found: False
90 in scores: True
```

- Verdict: stdlib. `list.sort` and `sorted` are stable, and the `bisect` module does binary search on a sorted list. `bisect_left` gives the first place where the value could go and `bisect_right` the place after equal items, so the two together give the range of equal items. Since Python 3.10 the functions take a `key` argument. `insort` finds the place in O(log n) steps, but the insertion shifts the tail of the list, so it is still O(n).
- The wrong block searches a list that is not sorted. `bisect_left` returns 5 and the program says that 90 is absent, although `90 in scores` is true. Nothing checks the precondition.
