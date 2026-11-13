```python run id=eager-and-lazy
from collections import Counter
from functools import reduce
from itertools import chain, islice

articles = [
    ("Intro to NoSQL", 1200, ["nosql", "databases"]),
    ("Event sourcing", 800, ["architecture"]),
    ("Graph stores", 1500, ["nosql", "graphs"]),
    ("Refactoring loops", 600, ["refactoring"]),
    ("Key-value stores", 900, ["nosql"]),
]

# filter, sort, slice and map: the titles of the two longest NoSQL articles.
nosql = [a for a in articles if "nosql" in a[2]]
print([t for t, _, _ in sorted(nosql, key=lambda a: a[1], reverse=True)[:2]])

# map and reduce: the total number of words in the NoSQL articles.
print(reduce(lambda acc, n: acc + n, map(lambda a: a[1], nosql), 0))

# flat map and group by: the number of articles for each tag.
counts = Counter(chain.from_iterable(tags for _, _, tags in articles))
print(sorted(counts.items()))

calls = 0


def score(article):
    global calls
    calls += 1
    return article[1]


# An eager pipeline makes a full list at each step, so it computes every score.
eager = [s for s in [score(a) for a in articles] if s > 1000][:1]
print("eager", eager, calls)

# A lazy pipeline of iterators stops after the first item that passes.
calls = 0
lazy = list(islice((s for s in map(score, articles) if s > 1000), 1))
print("lazy", lazy, calls)
```

```output
['Graph stores', 'Intro to NoSQL']
3600
[('architecture', 1), ('databases', 1), ('graphs', 1), ('nosql', 3), ('refactoring', 1)]
eager [1200] 5
lazy [1200] 1
```

- Verdict: native. Python has map, filter, sorted and functools.reduce as functions, so a pipeline nests calls or uses comprehensions for the steps. collections.Counter does the group by and count, and itertools.chain.from_iterable does the flat map. A list comprehension is eager and computes all 5 scores. A generator expression over map is lazy, and itertools.islice stops it after the first match, so score runs once.
