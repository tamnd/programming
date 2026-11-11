```python run id=recursive-cte
import sqlite3

db = sqlite3.connect(":memory:")
db.execute("CREATE TABLE parent (p TEXT, c TEXT)")
db.executemany("INSERT INTO parent VALUES (?, ?)",
               [("zeus", "ares"), ("ares", "harmonia"), ("harmonia", "semele"), ("semele", "dionysus")])

query = """
WITH RECURSIVE ancestor(a, d) AS (
    SELECT p, c FROM parent
    UNION
    SELECT parent.p, ancestor.d FROM parent JOIN ancestor ON parent.c = ancestor.a
)
SELECT d FROM ancestor WHERE a = 'zeus' ORDER BY d
"""
print([row[0] for row in db.execute(query)])
```

```output for=recursive-cte
['ares', 'dionysus', 'harmonia', 'semele']
```

```python wrong id=left-recursion
parent = [("zeus", "ares"), ("ares", "harmonia")]


def parent_of(x, y):
    for a, b in parent:
        if x in (None, a) and y in (None, b):
            yield a, b


def ancestor(x, y):
    for a, z in ancestor(x, None):
        for _, b in parent_of(z, y):
            yield a, b
    yield from parent_of(x, y)


try:
    print(list(ancestor("zeus", None)))
except RecursionError:
    print("RecursionError")
```

```output for=left-recursion
RecursionError
```

- Verdict: idiom. Python has no logic engine in the standard library, but generators give the backtracking search of Prolog (see the first block of the page), and SQLite's recursive common table expressions compute the same answers as Datalog rules.
- The `UNION` in the query removes duplicate rows, so the recursion stops when no new pair appears. This is the fixpoint of the rule.
- In the wrong block, the recursive rule calls itself before it reads any fact. A top-down search, as in Prolog, then calls `ancestor` forever. Put the fact first and the recursive goal last, or use a bottom-up engine such as Datalog, which does not have this problem.
