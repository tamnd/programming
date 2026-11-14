```python run id=hash-and-eq
# hash() of a small int is the int itself, and -1 is a special case.
print(hash(42), hash(-1), hash(-2))

# Equal numbers have equal hashes, so 1, 1.0 and True find the same entry.
print(hash(1) == hash(1.0) == hash(True), {1: "a"}[1.0])

# A str hash is salted per process: it is stable in one run, not across runs.
print(hash("key") == hash("key"))


# A key class defines __eq__ and __hash__ from the same fields.
class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y

    def __eq__(self, other):
        return isinstance(other, Point) and (self.x, self.y) == (other.x, other.y)

    def __hash__(self):
        return hash((self.x, self.y))


print({Point(1, 2): "found"}[Point(1, 2)])


# A class with __eq__ and no __hash__ is not hashable.
class OnlyEq:
    def __eq__(self, other):
        return True


try:
    {OnlyEq(): 1}
except TypeError as e:
    print(type(e).__name__)


# A bad hash still works, but every key goes to one bucket, so each lookup is a linear scan.
class Bad:
    calls = 0

    def __init__(self, n):
        self.n = n

    def __eq__(self, other):
        Bad.calls += 1
        return self.n == other.n

    def __hash__(self):
        return 0


d = {Bad(i): i for i in range(200)}
print(len(d), d[Bad(150)], Bad.calls > 10000)
```

```output
42 -2 -2
True a
True
found
TypeError
200 150 True
```

- Verdict: native. dict and set are hash tables. hash of a small int is the int, so equal numbers such as 1, 1.0 and True share a hash. A str hash is salted for each process. A key class defines __eq__ and __hash__ from the same fields, and a class with __eq__ and no __hash__ cannot be a key. A constant hash still works, but each lookup scans all keys.
