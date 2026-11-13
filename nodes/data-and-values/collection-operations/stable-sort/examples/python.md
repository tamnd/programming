```python run id=sort-passes-and-reverse
from operator import itemgetter

staff = [("ann", "ops", 3), ("bob", "dev", 2), ("cy", "ops", 2), ("di", "dev", 3), ("ed", "ops", 2)]

# Two passes: sort by the second key first, then by the first key.
# The stable second pass keeps the grade order inside each team.
s = sorted(staff, key=itemgetter(2))
s = sorted(s, key=itemgetter(1))
print([n for n, _, _ in s])

# One pass with a tuple key gives the same order.
print([n for n, _, _ in sorted(staff, key=itemgetter(1, 2))])

# reverse=True keeps equal items in their old order; it does not reverse them.
print([n for n, _, _ in sorted(staff, key=itemgetter(1), reverse=True)])

# Reversing the result of an ascending sort also reverses the equal items.
print([n for n, _, _ in reversed(sorted(staff, key=itemgetter(1)))])

# Several passes can use different directions: grade descending, then team ascending.
s = sorted(staff, key=itemgetter(2), reverse=True)
s = sorted(s, key=itemgetter(1))
print([(n, g) for n, _, g in s])
```

```output
['bob', 'di', 'cy', 'ed', 'ann']
['bob', 'di', 'cy', 'ed', 'ann']
['ann', 'cy', 'ed', 'bob', 'di']
['ed', 'cy', 'ann', 'di', 'bob']
[('di', 3), ('bob', 2), ('ann', 3), ('cy', 2), ('ed', 2)]
```

- Verdict: native. Two stable passes, grade first and team last, give the same order as the tuple key `(team, grade)`. `reverse=True` puts `ops` first and keeps `ann`, `cy` and `ed` in their input order. `reversed()` on an ascending sort gives the teams in the same order but reverses the equal items to `ed`, `cy`, `ann`. The last two passes sort by grade in descending order inside each ascending team.
